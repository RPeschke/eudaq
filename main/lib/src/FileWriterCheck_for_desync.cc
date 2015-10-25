#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <queue>
using namespace std;

#define SIGMA_CUT_OFF 40
#define MEAN_POSITION 1947.25

#define  size_of_my_array 10
struct planehit {
  planehit(double x_, double y_) :x(x_),y(y_){}
  double x, y;
};
class residual_maker {

};

struct mean_sigma {
  mean_sigma(double mean_, double sigma_) :mean(mean_), sigma(sigma_) {}
  double mean = 0, sigma = 0;
};

mean_sigma get_mean_sigma(const std::vector<double> & v) {
  double sum = std::accumulate(v.begin(), v.end(), 0.0);
  double mean = sum / v.size();

  double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / v.size() - mean * mean);

  return mean_sigma(mean, stdev);
}

namespace eudaq {



class fileWriterCheck_for_desync : public FileWriter {
public:
  fileWriterCheck_for_desync(const std::string &);
  virtual void StartRun(unsigned);
  virtual void WriteEvent(const DetectorEvent &);
  virtual uint64_t FileBytes() const;
  virtual ~fileWriterCheck_for_desync();
private:
  //  std::FILE * m_file;
  std::ofstream *m_out;
  bool firstEvent;
  vector<uint64_t> m_start_time;
  std::vector<double> m_res;
  int m_counter = 0;
  const int m_max_counter = 101;

  void run_planes(const StandardPlane& planeA, const StandardPlane& planeB);
  void run_plane(const planehit& planeA_hit, const StandardPlane& planeB);
  void push_to_vector(const planehit& planeA_hit, const planehit& planeB_hit);
  mean_sigma m_old_mean_sigma=mean_sigma(75,10000);

  static const int size_of_last = size_of_my_array;
  double m_last_sigmas[size_of_my_array] ;
  int last_index = 0;
  bool isSync = true;
  double m_avarage_sigma = 10000;
};


registerFileWriter(fileWriterCheck_for_desync, "desync");

size_t findPlaneById(const StandardEvent& sev, size_t id_) {
  for (size_t i = 0; i < sev.NumPlanes(); i++) {
    if (sev.GetPlane(i).ID() == id_) {
      return i;
    }

  }
  return sev.NumPlanes() + 1;
}

fileWriterCheck_for_desync::fileWriterCheck_for_desync(const std::string & param)
  :firstEvent(false), m_out(nullptr) {
  for (auto& e:m_last_sigmas)
  {
    e = 0;
  }
  std::cout << "EUDAQ_DEBUG: This is fileWriterCheck_for_desync::fileWriterCheck_for_desync(" << param << ")" << std::endl;
}

void fileWriterCheck_for_desync::StartRun(unsigned runnumber) {
  std::cout << "EUDAQ_DEBUG: fileWriterCheck_for_desync::StartRun(" << runnumber << ")" << std::endl;
  // close an open file
  if (m_out) {
    m_out->close();
    delete m_out;
    m_out = nullptr;

  }



  // open a new file
  std::string fname(FileNamer(m_filepattern).Set('X', ".txt").Set('R', runnumber));
  m_out = new std::ofstream(fname.c_str());

  if (!m_out) EUDAQ_THROW("Error opening file: " + fname);
}

void fileWriterCheck_for_desync::WriteEvent(const DetectorEvent & devent) {


  if (devent.IsBORE()) {
    eudaq::PluginManager::Initialize(devent);
    firstEvent = true;
    return;
  } else if (devent.IsEORE()) {
    return;
  }
  if (++m_counter > m_max_counter) {

    auto m = get_mean_sigma(m_res);
    *m_out << devent.GetEventNumber() << " ; " << m.mean << "; " << m.sigma << " ; " <<m_res.size()<< "\n";
#ifdef _DEBUG
    m_out->flush();
#endif // _DEBUG
    if (m.sigma>2*m_avarage_sigma)
    {
      std::cout << "losing sync after " << devent.GetEventNumber()<< " old sigma value "<<m_avarage_sigma<< "  new sigma value "<< m.sigma << std::endl;
      EUDAQ_THROW("losing sync");
    }
    if (fabs(m.sigma)<1000)
    {
      m_last_sigmas[last_index++] = m.sigma;
    }
    if (last_index>=size_of_last)
    {
      last_index = 0;
      m_avarage_sigma = std::accumulate(begin(m_last_sigmas), end(m_last_sigmas), 0)/size_of_last;
    }
    m_counter = 0;
    m_res.clear();
    m_old_mean_sigma = m;
  }
  StandardEvent sev = eudaq::PluginManager::ConvertToStandard(devent);


  auto DUT_id = findPlaneById(sev, 8);
  auto tel_id = findPlaneById(sev, 7);

  auto& dut_plane = sev.GetPlane(DUT_id);
  auto& tel_plane = sev.GetPlane(tel_id);
  run_planes(dut_plane, tel_plane);



}

fileWriterCheck_for_desync::~fileWriterCheck_for_desync() {
  if (m_out) {
    m_out->close();
    delete m_out;
    m_out = nullptr;
  }
}

void fileWriterCheck_for_desync::run_planes(const StandardPlane& planeA, const StandardPlane& planeB) {
  auto cds = planeA.GetPixels<double>();

  for (size_t ipix = 0; ipix < cds.size(); ++ipix) {
    run_plane(planehit(planeA.GetX(ipix), planeA.GetY(ipix)),planeB);

  }

}

void fileWriterCheck_for_desync::run_plane(const planehit& planeA_hit, const StandardPlane& planeB) {
  auto cds = planeB.GetPixels<double>();

  for (size_t ipix = 0; ipix < cds.size(); ++ipix) {

    planehit planeB_hit(planeB.GetX(ipix), planeB.GetY(ipix));

    push_to_vector(planeA_hit, planeB_hit);
  }

}

void fileWriterCheck_for_desync::push_to_vector(const planehit& planeA_hit, const planehit& planeB_hit) {


  auto res = planeA_hit.x - 3.19042*planeB_hit.x - MEAN_POSITION;

   if (res>m_old_mean_sigma.mean+SIGMA_CUT_OFF
       ||
       res<m_old_mean_sigma.mean - SIGMA_CUT_OFF
       )
   {
     return;
   }
  m_res.push_back(res);
}

uint64_t fileWriterCheck_for_desync::FileBytes() const { return m_out->tellp(); }

}

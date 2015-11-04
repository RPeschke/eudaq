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
#define peak_cutoff 5
#define max_peak_value 3
#define  size_of_my_array 10


struct planehit {
  planehit(double x_, double y_) :x(x_), y(y_) {}
  double x, y;
};
class residual_maker {

};

struct mean_sigma {
  mean_sigma(double mean_, double sigma_) :mean(mean_), sigma(sigma_) {}
  double mean = 0, sigma = 0;
};
template <size_t NBins>
class histoGram {
public:
  histoGram(double min_, double max_) :m_min(min_), m_max(max_) {
    m_step = (m_max - m_min) / NBins;

    for (size_t i = 0; i < NBins; ++i) {
      m_bins[i] = 0;
      m_axis[i] = m_min + i*m_step;
    }
  }

  void fill(double x) {
    int index = (x - m_min) / m_step;
    if (index >= NBins || index < 0) {
      return;
    }
    m_bins[index]++;
  }
  std::vector<int> getBins_sorted() {
    std::vector<int> ret(begin(m_bins), end(m_bins));

    std::sort(begin(ret), end(ret), std::greater<int>());

    return ret;
  }
  double m_min, m_max, m_step;

  int m_bins[NBins];
  double m_axis[NBins];
};
using histogram_t = histoGram<5000>;
mean_sigma get_mean_sigma(const std::vector<double> & v) {
  double sum = std::accumulate(v.begin(), v.end(), 0.0);
  double mean = sum / v.size();

  double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / v.size() - mean * mean);

  return mean_sigma(mean, stdev);
}

template <size_t Nbins>
class rollover_buffer {
public:
  rollover_buffer() {
    for (auto & e : m_buffer) {
      e = 0;
    }
  }
  void fill(double x) {
    if (m_index >= Nbins) {
      m_roll = true;
      m_index = 0;
    }
    m_buffer[m_index++] = x;
  }

  double get_mean() const {
    m_roll = false;
    double sum_ = 0, counter_ = 0;
    for (auto & e : m_buffer) {
      if (e > 0) {
        sum_ += e;
        ++counter_;
      }
    }
    return sum_ / counter_;
  }
  bool has_Rollover()const {
    return m_roll;
  }
private:
  mutable bool m_roll = false;
  int m_index = 0;
  double m_buffer[Nbins];
};
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
  mean_sigma m_old_mean_sigma = mean_sigma(75, 10000);

  static const int size_of_last = size_of_my_array;
  rollover_buffer<5> m_rollover;
  bool first_ = true;
  double m_avarage_peakness = 10000;
  histogram_t m_hist = histogram_t(-2000, 3000);
  void handle_x_th_event(const DetectorEvent& devent);
  void handle_x_th_event_cleanup();
  int num_OF_BAD_runs = 0;
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

    handle_x_th_event(devent);
    handle_x_th_event_cleanup();
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
    run_plane(planehit(planeA.GetX(ipix), planeA.GetY(ipix)), planeB);

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


  auto res = planeA_hit.x - 3.19042*planeB_hit.x;
  if (!first_) {
    if (res > m_old_mean_sigma.mean + SIGMA_CUT_OFF
        ||
        res < m_old_mean_sigma.mean - SIGMA_CUT_OFF
        ) {
      return;
    }
  }
  m_hist.fill(res);
  m_res.push_back(res);
}

void fileWriterCheck_for_desync::handle_x_th_event(const DetectorEvent& devent) {


  auto hiracy = m_hist.getBins_sorted();
  auto m = get_mean_sigma(m_res);
  double sum_ = std::accumulate(begin(hiracy), end(hiracy), 0);


  if (sum_ == 0) {
    return;
  }
  double peakness = m.sigma / ((double)hiracy[0]);
  *m_out << devent.GetEventNumber() << " ; " << m.mean << "; " << m.sigma << " ; " << m_res.size() << " ; " << hiracy[0] << "; " << peakness << "\n";
#ifdef _DEBUG
  m_out->flush();
#endif // _DEBUG
  if (peakness > peak_cutoff * m_avarage_peakness || peakness > max_peak_value) {

    std::cout << "losing sync after " << devent.GetEventNumber() << " old sigma value " << m_avarage_peakness << "  new sigma value " << peakness << std::endl;
    if (++num_OF_BAD_runs > 3) {
      EUDAQ_THROW("losing sync");
    }
    return;
  }

  if (fabs(peakness) > 1000) {
    return;
  }
  m_rollover.fill(peakness);
  if (m_rollover.has_Rollover()) {
    auto peak = m_rollover.get_mean();
    if (peak > 0) {
      m_avarage_peakness = peak;
    } else {
      std::cout << " peak was zero " << std::endl;
    }
  }
  if (num_OF_BAD_runs>0)
  {
  num_OF_BAD_runs--;
  }
  m_old_mean_sigma = m;
}

void fileWriterCheck_for_desync::handle_x_th_event_cleanup() {
  m_counter = 0;
  m_res.clear();
  m_hist = histogram_t(-2000, 3000);

  first_ = false;
}

uint64_t fileWriterCheck_for_desync::FileBytes() const { return m_out->tellp(); }

}

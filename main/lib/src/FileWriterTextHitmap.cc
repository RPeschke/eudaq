#ifdef USE_ROOT



#include "Windows4Root.h"
#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"

#if ((defined WIN32) && (defined __CINT__))
typedef unsigned long long uint64_t
typedef long long int64_t
typedef unsigned int uint32_t
typedef int int32_t
#else
#include <cstdint>
#endif
#include "eudaq/Configuration.hh"
#include "TCanvas.h"


template <typename T>
class pixelHit{
public:
  pixelHit(T x, T y) :m_x(x), m_y(y){}

  T m_x, m_y;
};

template <typename  T>
T hitDistance(const pixelHit<T>& h1, const pixelHit<T>& h2){
  T xdiff = (h1.m_x - h2.m_x);
  T ydiff = (h1.m_y - h2.m_y);
  T dif = sqrt(xdiff*xdiff + ydiff*ydiff);
  return dif;
}


template <typename T>
class Cluster{
public:
  using pixelHit_t = pixelHit < T > ;
    Cluster(const pixelHit_t& h){
    m_hits.push_back(h);
  }
  bool pushHit(const pixelHit_t& h){

    for (auto& e : m_hits){
      if (hitDistance(h, e) < 2)
      {
        m_hits.push_back(h);
        return true;
      }
    }
    return false;
  }
  const  pixelHit_t & getPos(){
    return m_hits.front();
  }
  std::vector<pixelHit_t> m_hits;
};
template <typename T>
class clusterMaker{
public:
  void reset(){
    m_cluster.clear();
  }
  size_t NumOfCluster(){
    return m_cluster.size();

  }

  size_t biggestCluster(){
    size_t s = 0;
    for (auto& e: m_cluster)
    {
      if (e.m_hits.size()>s)
      {
        s = e.m_hits.size();
      }
    }
    return s;
  }
  size_t smalestCluster(){
    size_t s = (size_t)-1;
    for (auto& e : m_cluster)
    {
      if (e.m_hits.size() < s)
      {
        s = e.m_hits.size();
      }
    }
    return s;
  }
  void push_pixel(const pixelHit<T>& h){
    bool clusterFound=false;
    for (auto& e : m_cluster)
    {

      if (e.pushHit(h))
      {
        clusterFound = true;
      }

    }

    if (!clusterFound)
    {
      m_cluster.emplace_back(h);
    }
  }


  

  std::vector<Cluster<T>> m_cluster;
};

namespace eudaq {
  class outputEvent{
  public:
    outputEvent(){


    }

    void pushChannel(Double_t channel_x, Double_t channel_y, Double_t Effi, Double_t NumberOfEvents,Double_t ID_)
    {
      m_id.push_back(ID_);
      m_x.push_back(channel_x);
      m_y.push_back(channel_y);
      m_occupancy.push_back(Effi);
      m_numOfEvents.push_back(NumberOfEvents);
    }
    void reset(){
      m_totalNumOfEvents = 0;
      m_total_efficiency = 0;
      m_residual = 0;
      m_offset = 0;
      m_Threshold = 0;
      m_RunNumber = 0;

       m_id.clear();
       m_x.clear();
       m_y.clear();
       m_occupancy.clear();
       m_numOfEvents.clear();
    }
   double m_totalNumOfEvents=0;
   double m_total_efficiency=0;
   double m_total_efficiency_2plus = 0;
   double m_residual=0;
   double m_offset=0;
   double m_Threshold=0;
   double m_RunNumber=0;
   double m_HV = 0;
   void Save2Tree(TTree* tree){
     tree->Branch("totalNumOfEvents", &m_totalNumOfEvents);
     tree->Branch("total_efficiency", &m_total_efficiency);
     tree->Branch("total_efficiency_2plus", &m_total_efficiency_2plus);
     tree->Branch("residual", &m_residual);
     tree->Branch("offset", &m_offset);
     tree->Branch("Threshold", &m_Threshold);
     tree->Branch("RunNumber", &m_RunNumber);
     tree->Branch("HV", &m_HV);


     tree->Branch("x", &m_x);
     tree->Branch("y", &m_y);
     tree->Branch("ID", &m_id);
     tree->Branch("Occupancy", &m_occupancy);
     tree->Branch("NumOfEvents", &m_numOfEvents);
   }
  private:
    std::vector<double> m_id;
    std::vector<double> m_x;
    std::vector<double> m_y;
    std::vector<double> m_occupancy;
    std::vector<double> m_numOfEvents;
  };
  class FileWriterTextHitmap : public FileWriter {
  public:
    FileWriterTextHitmap(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual uint64_t FileBytes() const;
    virtual ~FileWriterTextHitmap();
  private:

    void ProcessBORE(const DetectorEvent &);
    void ProcessEORE(const DetectorEvent &);
    void ProcessEvent(const DetectorEvent &);
    std::vector<unsigned> m_channel;
    unsigned m_orEvents = 0,m_clustersize_1=0,m_clustersize_larger1=0;


    Int_t m_threshold, m_threshold_readback, Hv;
    UInt_t m_run = 0, m_events;
    std::ofstream *m_out;
    bool firstEvent;
    void print();

    TFile *m_tfile = nullptr;
    TTree* m_tree = nullptr;

    clusterMaker<int> m_cluster;
    outputEvent m_outEvent;

    std::string hitmap_name;
    void reset();
  };


  registerFileWriter(FileWriterTextHitmap, "textHitmap");

  FileWriterTextHitmap::FileWriterTextHitmap(const std::string & param)
    :firstEvent(false), m_out(nullptr)
  {
    std::cout << "EUDAQ_DEBUG: This is FileWriterText::FileWriterText(" << param << ")" << std::endl;
  }

  void FileWriterTextHitmap::StartRun(unsigned runnumber) {
    std::cout << "EUDAQ_DEBUG: FileWriterText::StartRun(" << runnumber << ")" << std::endl;
    // close an open file
    if (m_out)
    {
      m_out->close();
      m_out = nullptr;
      m_tfile->Write();
      m_tfile->Close();
      delete m_tfile;
      m_tfile = nullptr;
    }




    // open a new file
    std::string fname(FileNamer(m_filepattern).Set('X', ".txt").Set('R', runnumber));
    m_out = new std::ofstream(fname.c_str());

    if (!m_out) EUDAQ_THROW("Error opening file: " + fname);

    std::string fname_root(FileNamer(m_filepattern).Set('X', ".root").Set('R', runnumber));
    m_tfile = new TFile(fname_root.c_str(), "RECREATE");

    m_tree = new TTree("hitmap", "hitmap");
    m_outEvent.Save2Tree(m_tree);

    hitmap_name = FileNamer("scurve$6R$X").Set('X', ".pdf").Set('R', runnumber);
    reset();

  }

  void FileWriterTextHitmap::WriteEvent(const DetectorEvent & devent) {

    if (devent.IsBORE()) {

      ProcessBORE(devent);   
      
      return;
    }
    else if (devent.IsEORE()) {
      ProcessEORE(devent);
     
      return;
    }

    ProcessEvent(devent);
  }


  FileWriterTextHitmap::~FileWriterTextHitmap() {
    print();

    if (m_out) {
      m_out->close();

      m_out = nullptr;
      TCanvas c1;
      c1.Divide(2, 1);
      c1.cd(1);
      m_tree->Draw("total_efficiency:Threshold", "", "*");
      auto pad = c1.cd(2);
      m_tree->Draw("Occupancy:Threshold", "", "colz");
      pad->SetLogz();
      c1.SaveAs(hitmap_name.c_str());
      m_tfile->Write();
      m_tfile->Close();
      delete m_tfile;
      m_tfile = nullptr;
    }
  }

  void FileWriterTextHitmap::ProcessBORE(const DetectorEvent &devent)
  {
    Configuration conf(devent.GetTag("CONFIG", ""), "Producer.ITS_ABC");

    if (!m_channel.empty())
    {
      print();
    }
    m_threshold_readback = 0;
    for (size_t i = 0; i < devent.NumEvents(); ++i)
    {
      auto st_vthr = devent.GetEvent(i)->GetTag("ST_VTHR ", -1.0);
      if (st_vthr > -1.0)
      {
        std::cout << " old style " << st_vthr << std::endl;
        m_threshold_readback = (int)st_vthr;
      }
      st_vthr = devent.GetEvent(i)->GetTag("ST_VTHR", -1.0);
      if (st_vthr > -1.0)
      {
        std::cout << " new style " << st_vthr << std::endl;
        m_threshold_readback = (int)st_vthr;
      }
    }
    m_threshold = conf.Get("ST_VTHR", 0);
    if (m_threshold != m_threshold_readback)
    {
      std::cout << "thresholds differ " << m_threshold << " != " << m_threshold_readback << std::endl;
    }
    Hv = conf.Get("ST_VDET", 0);
    m_run = devent.GetRunNumber();
    eudaq::PluginManager::Initialize(devent);
    firstEvent = true;
    m_events = 0;
    reset();
  }

  void FileWriterTextHitmap::ProcessEORE(const DetectorEvent &)
  {
    print();
    reset();
  }

  void FileWriterTextHitmap::ProcessEvent(const DetectorEvent & devent)
  {
    ++m_events;
    StandardEvent sev = eudaq::PluginManager::ConvertToStandard(devent);

    for (size_t iplane = 0; iplane < sev.NumPlanes(); ++iplane) {
      auto & plane = sev.GetPlane(iplane);

      if (plane.ID() == 8)
      {
        if (firstEvent)
        {

          m_channel.resize(plane.XSize() + 1);
          firstEvent = false;
        }
        bool hit = false;
        m_cluster.reset();
        for (size_t ipix = 0; ipix < plane.HitPixels(); ++ipix) {

          auto x = plane.GetX(ipix);
          if (x > 0)
          {
            m_channel[x]++;
            m_cluster.push_pixel(pixelHit<int>(x, 1));
            hit = true;
          }
        }
        if (hit)
        {
          ++m_orEvents;
          if (m_cluster.smalestCluster() == 1)
          {
            ++m_clustersize_1;
          }
          if (m_cluster.biggestCluster() > 1)
          {
            ++m_clustersize_larger1;
          }
        }
      }
    }
  }

  void FileWriterTextHitmap::print()
  {
    if (m_channel.empty())
    {
      return;
    }
    *m_out << "runNr = " << m_run
      << "   hv = " << Hv
      << " threshold = " << m_threshold
      << "  threshold_read_back =  " << m_threshold_readback
      << " Number_of_Events =  " << m_events << "  "
      << " total efficiency = " << (double)m_orEvents / m_events<< " ";
    int i = 0;
    m_outEvent.m_Threshold = m_threshold;
    m_outEvent.m_totalNumOfEvents = m_events;
    m_outEvent.m_total_efficiency = (double)m_orEvents / m_events ;
    m_outEvent.m_HV = Hv;
    m_outEvent.m_RunNumber = m_run;
    for (auto & e : m_channel){
      auto m_relhit = (double)e / m_events ;
      m_outEvent.pushChannel(i++, 0, m_relhit, m_events,0);
      *m_out << m_relhit << ", ";
      
    }
   




//     m_relhit = (double)m_clustersize_1 / m_events * 100;
//     m_channel_root = i++;
//     *m_out << m_relhit << ", ";
//     m_tree->Fill();


    m_outEvent.m_total_efficiency_2plus = (double)m_clustersize_larger1 / m_events ;

    *m_out << m_outEvent.m_total_efficiency_2plus << ", ";
    m_tree->Fill();
    
    *m_out << std::endl;
    m_channel.clear();
  }

  void FileWriterTextHitmap::reset()
  {
    m_events = 0;
    m_orEvents = 0;
    m_clustersize_1 = 0;
    m_clustersize_larger1 = 0;
    m_channel.clear();
    m_outEvent.reset();
  }

  uint64_t FileWriterTextHitmap::FileBytes() const { return m_out->tellp(); }

}
#endif // USE_ROOT
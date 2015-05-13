
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


namespace eudaq {

  class FileWriterTextHitmap : public FileWriter {
  public:
    FileWriterTextHitmap(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual uint64_t FileBytes() const;
    virtual ~FileWriterTextHitmap();
  private:
    std::vector<unsigned> m_channel;

    Int_t m_threshold, m_threshold_readback, Hv;
    UInt_t m_run = 0,m_events;
    std::ofstream *m_out;
    bool firstEvent;
    void print();

    TFile *m_tfile=nullptr;
    TTree* m_tree = nullptr;

    Double_t m_relhit;
    Int_t m_channel_root;
    std::string hitmap_name;
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
    m_tfile = new TFile(fname_root.c_str(),"RECREATE");

    m_tree = new TTree("hitmap","hitmap");
    m_tree->Branch("Channel", &m_channel_root);
    m_tree->Branch("hv", &Hv);
    m_tree->Branch("threshold", &m_threshold);
    m_tree->Branch("threshold_readback", &m_threshold_readback);
    m_tree->Branch("run", &m_run);
    m_tree->Branch("relHit", &m_relhit);

    hitmap_name = FileNamer("scurve$6R$X").Set('X', ".pdf").Set('R', runnumber);


  }

  void FileWriterTextHitmap::WriteEvent(const DetectorEvent & devent) {

    if (devent.IsBORE()) {

      Configuration conf(devent.GetTag("CONFIG", ""),"Producer.ITS_ABC");

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
      if (m_threshold!=m_threshold_readback)
      {
        std::cout << "thresholds differ " << m_threshold << " != " << m_threshold_readback << std::endl;
      }
      Hv = conf.Get("ST_VDET", 0);
      m_run = devent.GetRunNumber();
      eudaq::PluginManager::Initialize(devent);
      firstEvent = true;
      m_events = 0;
      return;
    }
    else if (devent.IsEORE()) {

      print();
      return;
    }
    ++m_events;
    StandardEvent sev = eudaq::PluginManager::ConvertToStandard(devent);

    for (size_t iplane = 0; iplane < sev.NumPlanes(); ++iplane) {
      auto & plane = sev.GetPlane(iplane);

      if (plane.ID() == 8)
      {
        if (firstEvent)
        {

          m_channel.resize(plane.XSize()+2);
          firstEvent = false;
        }
        bool hit = false;
        for (size_t ipix = 0; ipix < plane.HitPixels(); ++ipix) {

          auto x = plane.GetX(ipix);
          if (x > 0)
          {

            m_channel[x]++;
            hit = true;
          }
        }
        if (hit)
        {
          m_channel.back()++;
        }
      }
    }
  }


    FileWriterTextHitmap::~FileWriterTextHitmap() {
      print(); 
 
      if (m_out) {
        m_out->close();

        m_out = nullptr;
        TCanvas c1;
        c1.Divide(2, 1);
        c1.cd(1);
        m_tree->Draw("relHit:threshold", "Channel==385", "*");
        auto pad=c1.cd(2);
        m_tree->Draw("relHit:threshold", "Channel<385", "colz");
        pad->SetLogz();
        c1.SaveAs(hitmap_name.c_str());
        m_tfile->Write();
        m_tfile->Close();
        delete m_tfile;
        m_tfile = nullptr;
      }
    }

    void FileWriterTextHitmap::print()
    {
      if (m_channel.empty())
      {
        return;
      }
      *m_out << "runNr = " << m_run << "   hv = " << Hv << " threshold = " << m_threshold << "  threshold_read_back =  " << m_threshold_readback<<"  ";
      int i = 0;
      for (auto & e : m_channel){
        *m_out << (double)e /m_events *100<< ", ";
        m_relhit = (double)e / m_events * 100;
        m_channel_root = i++;
        m_tree->Fill();
      }

      *m_out << std::endl;
      m_channel.clear();
    }

    uint64_t FileWriterTextHitmap::FileBytes() const { return m_out->tellp(); }

}
  
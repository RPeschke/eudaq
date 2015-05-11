#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>
#include <fstream>

#if ((defined WIN32) && (defined __CINT__))
typedef unsigned long long uint64_t
typedef long long int64_t
typedef unsigned int uint32_t
typedef int int32_t
#else
#include <cstdint>
#endif
#include "eudaq/Configuration.hh"


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

    int m_threshold, Hv;
    unsigned m_run = 0,m_events;
    std::ofstream *m_out;
    bool firstEvent;
    void print();
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

    }



    // open a new file
    std::string fname(FileNamer(m_filepattern).Set('X', ".txt").Set('R', runnumber));
    m_out = new std::ofstream(fname.c_str());

    if (!m_out) EUDAQ_THROW("Error opening file: " + fname);
  }

  void FileWriterTextHitmap::WriteEvent(const DetectorEvent & devent) {

    if (devent.IsBORE()) {
      
      Configuration conf(devent.GetTag("CONFIG", ""),"Producer.ITS_ABC");

      if (!m_channel.empty())
      {
        print();
      }
      m_threshold = conf.Get("ST_VTHR", 0);
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
      }
    }

    void FileWriterTextHitmap::print()
    {
      if (m_channel.empty())
      {
        return;
      }
      *m_out << "runNr = " << m_run<< "   hv = " << Hv << " threshold = " << m_threshold << "   ";
      for (auto & e : m_channel){
        *m_out << (double)e /m_events *100<< ", ";
      }

      *m_out << std::endl;
      m_channel.clear();
    }

    uint64_t FileWriterTextHitmap::FileBytes() const { return m_out->tellp(); }

}
  
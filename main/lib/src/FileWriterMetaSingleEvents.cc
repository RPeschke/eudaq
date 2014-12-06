

#include "eudaq/FileNamer.hh"
#include "eudaq/FileWriter.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/Logger.hh"
#include <iostream>
#include <fstream>
#include "eudaq/Configuration.hh"



namespace eudaq {

  class FileWriterMetaSingleEvents : public FileWriter {
  public:
    FileWriterMetaSingleEvents(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &){}
    virtual void WriteBaseEvent(const Event &);
    virtual uint64_t FileBytes() const;
    virtual ~FileWriterMetaSingleEvents();
  private:
    std::ofstream *m_out;
    bool firstEvent;
    uint64_t DUT_start_time;


  };

  namespace {
    static RegisterFileWriter<FileWriterMetaSingleEvents> reg("smeta");
  }

  FileWriterMetaSingleEvents::FileWriterMetaSingleEvents(const std::string & /*param*/)  {
  }

  void FileWriterMetaSingleEvents::StartRun(unsigned runnumber) {
    EUDAQ_INFO("Converting the inputfile into a Text ");
    std::string foutput(FileNamer(m_filepattern).Set('X', ".txt").Set('R', runnumber));
    EUDAQ_INFO("Preparing the outputfile: " + foutput);

    m_out = new std::ofstream(foutput);


  }

  void FileWriterMetaSingleEvents::WriteBaseEvent(const Event & ev) {
    if (ev.IsBORE()) {
//      Configuration conf{};
      eudaq::PluginManager::Initialize(dynamic_cast<const DetectorEvent&>(ev));
      return;
    }
    else if (ev.IsEORE()) {

    }

    *m_out << "<event type = \"" << Event::id2str(ev.get_id()) << "\" subEvent = \"" << ev.GetSubType() << "\">\n";
    for (size_t i = 0; i < ev.GetSizeOfTimeStamps(); ++i)
    {
      *m_out << ev.GetTimestamp(i) << "\n";
    }

    *m_out << "</event> \n";


  }

  FileWriterMetaSingleEvents::~FileWriterMetaSingleEvents(){


  }

  uint64_t FileWriterMetaSingleEvents::FileBytes() const { return 0; }


}

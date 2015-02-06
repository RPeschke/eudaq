#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>
#include <fstream>



namespace eudaq {

  class FileWriterXmlCompact : public FileWriter {
  public:
    FileWriterXmlCompact(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual uint64_t FileBytes() const;
    virtual ~FileWriterXmlCompact();
  private:
    //  std::FILE * m_file;
    std::ofstream *m_out;
    bool firstEvent;
    uint64_t DUT_start_time,TLU_start_Time;
  };

  
  registerFileWriter(FileWriterXmlCompact, "xmlc");
  
  FileWriterXmlCompact::FileWriterXmlCompact(const std::string & param)
    :firstEvent(false), m_out(nullptr), DUT_start_time(0)
  {
    std::cout << "EUDAQ_DEBUG: This is FileWriterXmlCompact::FileWriterXmlCompact(" << param << ")" << std::endl;
  }

  void FileWriterXmlCompact::StartRun(unsigned runnumber) {
    std::cout << "EUDAQ_DEBUG: FileWriterXmlCompact::StartRun(" << runnumber << ")" << std::endl;
    // close an open file
    if (m_out)
    {
      m_out->close();
      m_out = nullptr;

    }



    // open a new file
    std::string fname(FileNamer(m_filepattern).Set('X', ".xml").Set('R', runnumber));
    m_out = new std::ofstream(fname.c_str());
    *m_out << "<events>\n";
    if (!m_out) EUDAQ_THROW("Error opening file: " + fname);
  }

  void FileWriterXmlCompact::WriteEvent(const DetectorEvent & devent) {


    if (devent.IsBORE()) {
      eudaq::PluginManager::Initialize(devent);
      firstEvent = true;
      return;
    }
    else if (devent.IsEORE()) {
      return;
    }
    StandardEvent sev = eudaq::PluginManager::ConvertToStandard(devent);


    if (firstEvent)
    {


        //*m_out << "i_time_stamp;  TLU_trigger; DUT_time_stamp" << std::endl;

      auto ev = devent.GetEvent(0);
      DUT_start_time = ev->GetTimestamp(0) - 50000;
      TLU_start_Time = ev->GetTimestamp(1) - 50000;
      firstEvent = false;
    }
 
    *m_out << "<event>\n";

    for (size_t i = 0; i < devent.NumEvents();++i)
    {
      auto ev = devent.GetEvent(i);
      *m_out << "  <EventType>" << Event::id2str(ev->get_id()) << "</EventType>\n";
      *m_out << "  <EventSubType>" << ev->GetSubType() << "</EventSubType>\n";
      *m_out << "     <eventNr>" << ev->GetEventNumber() << "</eventNr>\n";
      *m_out << "     <timeStamps>\n";
      for (size_t j = 0; j < ev->GetSizeOfTimeStamps();++j)
      {
        if (j==0)
        {
          *m_out << "       <timeStamp>" << ev->GetTimestamp(j) - DUT_start_time << "</timeStamp>\n";
        }
        else
        {
          *m_out << "       <timeStamp>" << ev->GetTimestamp(j) - TLU_start_Time << "</timeStamp>\n";
        }
      }
      
      *m_out << "     </timeStamps>\n";
    }
    
    *m_out << "</event>\n";

    //*m_out << sev.GetTimestamp() - TLU_start_Time << "; " << sev.GetTag("TLU_trigger") << ";" << sev.GetTag("TLU_event_nr",(uint64_t)0) << "; " << sev.GetTag("ni_time", (uint64_t) 0) - DUT_start_time << "; " << sev.GetTag("ni_event_nr", (uint64_t) 0) << std::endl;

  
    
//     for (size_t iplane = 0; iplane < sev.NumPlanes(); ++iplane) {
// 
//       const eudaq::StandardPlane & plane = sev.GetPlane(iplane);
//       std::vector<double> cds = plane.GetPixels<double>();
// 
// 
//       //          if (ipix < 10) std::cout << ", " << plane.m_pix[0][ipix] << ";" << cds[ipix]
//       *m_out << sev.GetTimestamp() -TLU_start_Time << "; ";
//       *m_out << sev.GetTag("TLU_trigger") << "; ";
//       *m_out << plane.ID() << "; ";
//       *m_out << plane.TLUEvent() << "; ";
//       *m_out << sev.GetEventNumber() << "; ";
//       *m_out << sev.GetTag("ni_time", (uint64_t) 0) - DUT_start_time << "; ";
//       //std::string dummy=sev.GetTag("TLU_input");
//       //		*m_out<<std::stoi(sev.GetTag("TLU_input"))<<"; ";
//       *m_out << std::endl;
// 
//     }
#ifdef _DEBUG

    m_out->flush();
#endif // DEBUG

  }

  FileWriterXmlCompact::~FileWriterXmlCompact() {
    if (m_out) {
      *m_out << "</events>";
      m_out->close();

      m_out = nullptr;
    }
  }

  uint64_t FileWriterXmlCompact::FileBytes() const { return m_out->tellp(); }

}

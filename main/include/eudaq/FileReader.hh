#ifndef EUDAQ_INCLUDED_FileReader
#define EUDAQ_INCLUDED_FileReader

#include "eudaq/FileSerializer.hh"
#include "eudaq/DetectorEvent.hh"
#include "eudaq/StandardEvent.hh"
//#include "eudaq/counted_ptr.hh"
#include <string>
#include "EventSynchronisationBase.hh"
#include <memory>

namespace eudaq {
	class eventqueue_t;

  class DLLEXPORT FileReader {
    public:
      FileReader(const std::string & filename, const std::string & filepattern = "");
	//  FileReader(FileReader&& fileR);
      ~FileReader();
      bool NextEvent(size_t skip = 0);
      std::string Filename() const { return m_filename; }
      unsigned RunNumber() const;
      const eudaq::Event & GetEvent() const;
      const DetectorEvent & Event() const { return GetDetectorEvent(); } // for backward compatibility
      const DetectorEvent & GetDetectorEvent() const;
      const StandardEvent & GetStandardEvent() const;
	  std::shared_ptr<eudaq::DetectorEvent> GetDetectorEvent_ptr(){return std::dynamic_pointer_cast<eudaq::DetectorEvent>(m_ev);};
      void Interrupt() { m_des.Interrupt(); }
      
    private:
      std::string m_filename;
      FileDeserializer m_des;
     std::shared_ptr<eudaq::Event> m_ev;
      unsigned m_ver;
	//  std::shared_ptr<eudaq::SyncBase> m_sync;
       
	  //delete copy constructor;
	//  FileReader(const FileReader&);
  };
 


}

#endif // EUDAQ_INCLUDED_FileReader

#ifndef MuliFileReader_h__
#define MuliFileReader_h__

#include "Platform.hh"


#include <string>
#include <memory>
#include <vector>

#include "eudaq/EventSynchronisationBase.hh"
#include "eudaq/baseFileReader.hh"
#include "eudaq/OptionParser.hh"


namespace eudaq{
	class Event;
	class DetectorEvent;
  
	class DLLEXPORT multiFileReader:public baseFileReader{
	public:
		multiFileReader(bool sync=true);

		 unsigned RunNumber() const;

		bool NextEvent(size_t skip = 0);
		std::string Filename() const { return m_filename; }
		   const DetectorEvent & GetDetectorEvent() const;
		   const eudaq::Event & GetEvent() const;
	
		void addFileReader(const std::string & filename, const std::string & filepattern = "");
    void addSyncAlgorithm(std::unique_ptr<SyncBase> sync);
    void addSyncAlgorithm(SyncBase::MainType type = "", SyncBase::Parameter_ref sync= 0);
		void Interrupt() ;
    using strOption_ptr = std::unique_ptr < eudaq::Option<std::string> >;
    static strOption_ptr  add_Command_line_option_inputPattern(OptionParser & op);
    static std::string help_text();
	private:
		std::string m_filename;
		std::shared_ptr<eudaq::Event> m_ev;
		std::vector<std::unique_ptr<eudaq::baseFileReader>> m_fileReaders;
		std::unique_ptr<SyncBase> m_sync;
		size_t m_eventsToSync;
		bool m_preaparedForEvents;
		
	};

}


#endif // MuliFileReader_h__

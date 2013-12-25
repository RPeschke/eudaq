#include "eudaq/FileReader.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/Event.hh"
#include "eudaq/Logger.hh"
#include "eudaq/EventQueue.hh"
#include <list>
#include "eudaq/FileSerializer.hh"

namespace eudaq {

  namespace {
    static const unsigned TLUID = Event::str2id("_TLU");
    static const unsigned IDMASK = 0x7fff;
  }

  std::ostream & operator << (std::ostream &, const eventqueue_t &);


  std::ostream & operator << (std::ostream & os, const eventqueue_t & q) {
    q.debug(os);
    return os;
  }



  FileReader::FileReader(const std::string & file, const std::string & filepattern, bool synctriggerid)
    : m_filename(FileNamer(filepattern).Set('X', ".raw").SetReplace('R', file)),
    m_des(m_filename),
    m_ev(EventFactory::Create(m_des)),
    m_ver(1)
    {
      //unsigned versiontag = m_des.peek<unsigned>();
      //if (versiontag == Event::str2id("VER2")) {
      //  m_ver = 2;
      //  m_des.read(versiontag);
      //} else if (versiontag != Event::str2id("_DET")) {
      //  EUDAQ_WARN("Unrecognised native file (tag=" + Event::id2str(versiontag) + "), assuming version 1");
      //}
      //EUDAQ_INFO("FileReader, version = " + to_string(m_ver));
      //NextEvent();
      if (synctriggerid) {
        
		m_sync =std::shared_ptr<eudaq::SyncBase>(new eudaq::SyncBase(GetDetectorEvent().NumEvents()));
      }
    }

  FileReader::~FileReader() {
    
  }

  bool FileReader::NextEvent(size_t skip) {
    std::shared_ptr<eudaq::Event> ev = 0;
    if (m_sync) {
      bool result = false;
      for (size_t i = 0; i <= skip; ++i) {
        if (!m_sync->SynEvents(m_des, m_ver, ev)) break;
        result = true;
      }
      if (ev) {
        m_ev =(ev);
      }
      return result;
    }
	// !m_sync  
    bool result = m_des.ReadEvent(m_ver, ev, skip);
    if (ev) m_ev =  std::shared_ptr<eudaq::Event>(ev);
    return result;
  }

  unsigned FileReader::RunNumber() const {
    return m_ev->GetRunNumber();
  }

  const Event & FileReader::GetEvent() const {
    return *m_ev;
  }

  const DetectorEvent & FileReader::GetDetectorEvent() const {
    return dynamic_cast<const DetectorEvent &>(*m_ev);
  }

  const StandardEvent & FileReader::GetStandardEvent() const {
    return dynamic_cast<const StandardEvent &>(*m_ev);
  }

  //   const StandardEvent & FileReader::GetStandardEvent() const {
  //     if (!m_sev) {
  //       counted_ptr<StandardEvent> sevent(new StandardEvent);
  //       const DetectorEvent & dev = GetDetectorEvent();
  //       for (size_t i = 0; i < dev.NumEvents(); ++i) {
  //         const eudaq::Event * subevent = dev.GetEvent(i);

  //         try {
  //           const DataConverterPlugin * converterplugin = PluginManager::GetInstance().GetPlugin(subevent->GetType());
  //           converterplugin->GetStandardSubEvent(*sevent, *subevent);
  //           //std::fprintf(m_file, "Event %d %d\n", devent.GetEventNumber(), standardevent->m_x.size());
  //         } catch(eudaq::Exception & e) {
  //           //std::cout <<  e.what() << std::endl;
  //           std::cout <<  "FileWriterText::WriteEvent(): Ignoring event type "
  //                     <<  subevent->GetType() << std::endl;
  //           continue;
  //         }
  //       }
  //       m_sev = sevent;
  //     }
  //     return *m_sev;
  //   }

}

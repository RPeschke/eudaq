#include <list>
#include "eudaq/baseFileReader.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/Event.hh"
#include "eudaq/Logger.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/factory.hh"

namespace eudaq {

  class DLLEXPORT FileReader_primarTLU : public baseFileReader {
  public:
    FileReader_primarTLU(const std::string & filename, const std::string & filepattern = "");

    FileReader_primarTLU(Parameter_ref param);


    ~FileReader_primarTLU();
    virtual  unsigned RunNumber() const;
    virtual bool NextEvent(size_t skip = 0);
    virtual std::shared_ptr<eudaq::Event> getEventPtr() { return m_ev; }
    virtual std::shared_ptr<eudaq::Event> GetNextEvent();
    virtual void Interrupt() { m_des.Interrupt(); }



    const eudaq::Event & GetEvent() const;
    const DetectorEvent & Event() const { return GetDetectorEvent(); } // for backward compatibility
    const DetectorEvent & GetDetectorEvent() const;
    const StandardEvent & GetStandardEvent() const;
    std::shared_ptr<eudaq::DetectorEvent> GetDetectorEvent_ptr(){ return std::dynamic_pointer_cast<eudaq::DetectorEvent>(m_ev); };

  private:

    bool isSyncEvent(const eudaq::Event& ev) const{
      
      auto det = dynamic_cast<const DetectorEvent*>(&ev);
      for (size_t i = 0; i < det->NumEvents(); ++i){
        auto& ev = *det->GetEvent(i);
        if (PluginManager::isTLU(ev)){
            return (ev.GetTag("trigger", 0) == 1);
        }
      }

      return false;
    }

    FileDeserializer m_des;
    std::shared_ptr<eudaq::Event> m_ev,m_ev1;
    unsigned m_ver;
    size_t m_subevent_counter = 0;

  };




  FileReader_primarTLU::FileReader_primarTLU(const std::string & file, const std::string & filepattern)
    : baseFileReader(FileNamer(filepattern).Set('X', ".raw").SetReplace('R', file)),
    m_des(Filename()),
    m_ev(EventFactory::Create(m_des)),
    m_ver(1)
  {


  }

  FileReader_primarTLU::FileReader_primarTLU(Parameter_ref param) :FileReader_primarTLU(param.Get(getKeyFileName(),""), param.Get(getKeyInputPattern(),""))
  {

  }

  FileReader_primarTLU::~FileReader_primarTLU() {

  }

  bool FileReader_primarTLU::NextEvent(size_t skip) {
    std::shared_ptr<eudaq::Event> ev = nullptr;

    if (!m_ev1)
    {
      // First Event
      bool result = m_des.ReadEvent(m_ver, ev, skip);
      m_ev = ev;
      ev = nullptr;
    }
    else
    {
      // all other events 
      m_ev = m_ev1;
    }
    bool result = m_des.ReadEvent(m_ver, ev, skip);
    
    if (ev) {
      if (!isSyncEvent(*ev))
      {
        // normal case 
        m_ev1 = ev;

      }
      else
      {
        // sync events reverse order 

        m_ev1 = m_ev;
        m_ev = ev;
      }
    
    
    }

    return result;
  }

  unsigned FileReader_primarTLU::RunNumber() const {
    return m_ev->GetRunNumber();
  }

  const Event & FileReader_primarTLU::GetEvent() const {
    return *m_ev;
  }

  const DetectorEvent & FileReader_primarTLU::GetDetectorEvent() const {
    return dynamic_cast<const DetectorEvent &>(*m_ev);
  }

  const StandardEvent & FileReader_primarTLU::GetStandardEvent() const {
    return dynamic_cast<const StandardEvent &>(*m_ev);
  }

  std::shared_ptr<eudaq::Event> FileReader_primarTLU::GetNextEvent(){

    if (!NextEvent()) {
      return nullptr;
    }

    return m_ev;


  }

  RegisterFileReader(FileReader_primarTLU, "primarTLU");

}

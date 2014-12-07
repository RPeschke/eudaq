#include "eudaq/FileReader.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/Event.hh"
#include "eudaq/Logger.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/Configuration.hh"

#include <list>

namespace eudaq {





  FileReader::FileReader(const std::string & file, const std::string & filepattern)
    : baseFileReader(FileNamer(filepattern).Set('X', ".raw").SetReplace('R', file)),
    m_des(Filename()),
    m_ev(EventFactory::Create(m_des)),
    m_ver(1)
  {

  }

  FileReader::~FileReader() {

  }

  bool FileReader::NextEvent(size_t skip) {
    std::shared_ptr<eudaq::Event> ev = nullptr;


    bool result = m_des.ReadEvent(m_ver, ev, skip);
    if (ev) m_ev = ev;
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

  std::shared_ptr<eudaq::Event> FileReader::GetNextEvent(){
    if (m_ev->IsPacket())
    {

      if (m_subevent_counter == PluginManager::GetNumberOfROF(*m_ev)){
        m_subevent_counter = 0;
        if (!NextEvent()) {
          return nullptr;
        }
        return PluginManager::ExtractEventN(m_ev, m_subevent_counter++);
      }
    }
    else{
      return m_ev;
    }

  }

  bool FileIsEUDET(const std::string& in)
  {
    auto pos_of_Dot = in.find_last_of('.');
    if (pos_of_Dot < in.size())
    {
      auto sub = in.substr(pos_of_Dot + 1);
      if (sub.compare("raw") == 0)
      {
        return true;
      }
    }

    return false;
  }



}

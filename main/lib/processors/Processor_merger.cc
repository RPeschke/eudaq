#include "eudaq/Processor_merger.hh"
#include "eudaq/EventSynchronisationBase.hh"

namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;






  ReturnParam Processor_merger::ProcessEvent(event_sp ev, ConnectionName_ref name) {
    auto it = m_map.find(name);
    size_t index = 0;
    if (it == m_map.end()) {
      index = m_counter;
      m_map[name] = m_counter++;

    } else {
      index = it->second;
    }

    if (!m_sync->pushEvent(ev, index)) {

      return ProcessorBase::stop;
    }
    if (m_sync->InputIsEmpty()) {
      return ProcessorBase::sucess; //more events needed 
    }
    ev = nullptr;
    if (m_sync->getNextEvent(ev)) {
      return processNext(std::move(ev), default_connection());
    }

    return ProcessorBase::sucess;
  }


  Processor_merger::Processor_merger(ProcessorBase::Parameter_ref name, const SyncBase::MainType& type_, SyncBase::Parameter_ref param_) : ProcessorBase(name), m_type(type_), m_param(param_) {

  }

  void Processor_merger::init()
  {
    m_counter = 0;
    m_map.clear();
    m_sync = EventSyncFactory::create(m_type,m_param);
  }

  void Processor_merger::end()
  {

  }


}
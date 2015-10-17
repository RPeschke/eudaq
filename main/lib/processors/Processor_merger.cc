#include "eudaq/Processor_merger.hh"
#include "eudaq/EventSynchronisationBase.hh"

namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;







  ReturnParam Processor_merger::ProcessEvent(ConnectionName_ref name, event_sp ev)
  {
    auto it = m_map.find(name);
    size_t index = 0;
    if (it == m_map.end()){
      index = m_counter;
      m_map[name] = m_counter++;

    } else {
      index =it->second;
    }

    if (!m_sync->pushEvent(ev, index)){

      return ProcessorBase::stop;
    }
    if (m_sync->InputIsEmpty())
    {
      return ProcessorBase::sucess; //more events needed 
    }
    ev = nullptr;
    if (m_sync->getNextEvent(ev))
    {
      return ProcessNext("", std::move(ev));
    }
    
    return ProcessorBase::sucess;
  }

  Processor_merger::Processor_merger(Parameter_ref conf) :Processor_N_x_M(conf)
  {

  }

  void Processor_merger::initialize()
  {
    m_counter = 0;
    m_map.clear();
    SyncBase::Parameter_t p;
    m_sync = EventSyncFactory::create("DetectorEvents", p);
  }

  void Processor_merger::finish()
  {

  }


}
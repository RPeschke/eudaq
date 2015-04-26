#include "eudaq/Processor_merger.hh"
#include "eudaq/EventSynchronisationBase.hh"

namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;




  RegisterProcessor(Processor_merger, ProcessorNames::merger());



  ReturnParam Processor_merger::ProcessorEvent(ConnectionName_ref name, event_sp ev)
  {
    auto it = m_map.find(name);
    if (it == m_map.end()){

      m_map[name] = m_counter++;
    }

    if (!m_sync->pushEvent(ev, m_map[name])){

      return ProcessorBase::stop;
    }
    if (m_sync->InputIsEmpty())
    {
      return ProcessorBase::sucess; //more events needed 
    }
    ev = nullptr;
    if (m_sync->getNextEvent(ev))
    {
      return ProcessNext("", ev);
    }
    
    return ProcessorBase::sucess;
  }

  Processor_merger::Processor_merger(Parameter_ref conf) :Processor_N_x_M(conf)
  {

  }

  void Processor_merger::init()
  {
    m_counter = 0;
    m_map.clear();
    SyncBase::Parameter_t p;
    m_sync = EventSyncFactory::create("DetectorEvents", p);
  }

}
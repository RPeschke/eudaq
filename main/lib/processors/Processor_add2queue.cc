#include "eudaq/Processor_add2queue.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  eudaq::Processor_add2queue::Processor_add2queue(Parameter_ref conf) :Processor(conf)
  {

  }

  ReturnParam eudaq::Processor_add2queue::ProcessorEvent(event_sp ev)
  {
    ReturnParam ret =sucess;
    if (ev)
    {
      ret = ProcessNext(ev);
    }
     if (ret!= sucess)
    {
      return ret;
    }

     if (!ev&& m_last_ret==stop)
     {
       return stop;
     }
    ev = nullptr;

    if (m_last_ret==stop)
    {
      
      return ProcessNext(ev);;
    }
    ret = add2queue(ev);
    if (ret != sucess)
    {
      return ret;
    }
    m_last_ret = ProcessNext(ev); //only stopping this producer 
    return sucess;
  }
}
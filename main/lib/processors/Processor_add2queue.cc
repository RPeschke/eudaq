#include "eudaq/Processor_add2queue.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  eudaq::Processor_add2queue::Processor_add2queue(Parameter_ref conf) :Processor(conf)
  {
    m_status = running;
  }

  ReturnParam eudaq::Processor_add2queue::ProcessorEvent(event_sp ev)
  {
    if (m_status==running)
    {
      event_sp ev1;
      auto ret = add2queue(ev1);
      if (ret==stop)
      {
        m_status = stopping;
      }
      if (ret != sucess)
      {
        return ret;
      }
      handelReturn(ProcessNext(std::move(ev1))); 



    }
    if (ev)
    {
      return ProcessNext(std::move(ev));
    }else if (m_status==stopping)
    {
      return ProcessNext(std::move(ev));
    }
     
    return sucess;
  }

  void Processor_add2queue::handelReturn(ReturnParam ret)
  {
    m_last_ret = ret;
    switch (ret)
    {
    case eudaq::ProcessorBase::stop:
      m_status = stopping;
      break;
    default:
      break;
    }

    
  }

}
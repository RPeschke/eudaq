#include "eudaq/Processor_Buffer.hh"




namespace eudaq{
  RegisterProcessor(Proccessor_Buffer, ProcessorNames::buffer());
  using ReturnParam = ProcessorBase::ReturnParam;



  Proccessor_Buffer::Proccessor_Buffer(Parameter_ref conf) :Processor(conf)
  {

  }

  ReturnParam Proccessor_Buffer::ProcessorEvent(event_sp ev)
  {
    if (m_queue.size()>m_bufferSize)
    {
      return ProcessorBase::busy_skip;
    }

    if (m_queue.empty())
    {
      auto ret= ProcessNext(ev);
      if (ret==ProcessorBase::busy_retry)
      {
        m_queue.push_back(ev);
        return ProcessorBase::sucess;
      }
      return ret;
      
    }
    else
    {
      m_queue.push_back(ev);
    }
    ReturnParam ret = ProcessorBase::sucess;
    while (!m_queue.empty() )
    {
      auto ev1 = m_queue.front();
      ret = ProcessNext(ev1);
      if (ret != busy_retry)
      {
        m_queue.pop_front();
      }
      else if (ret ==ProcessorBase::busy_retry)
      {
        return ProcessorBase::sucess;
      }else if(ret!= ProcessorBase::sucess)
      {
        return ret;
      }

    }
    return ProcessorBase::sucess;
  }

  void Proccessor_Buffer::init()
  {
    m_queue.clear();
  }

}
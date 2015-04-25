#include "eudaq/Processor_busy_test.hh"

namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;
  RegisterProcessor(Processor_busy_test, ProcessorNames::busy_test());
  Processor_busy_test::Processor_busy_test(Parameter_ref conf) :Processor_Inspector(conf)
  {

  }

  ReturnParam Processor_busy_test::inspecktEvent(const Event&)
  {
    ++m_counter;
    if (m_counter<10)
    {
      return ProcessorBase::busy_retry;
    }
//     if (m_counter % 3 == 0){
//       return ProcessorBase::busy_retry;
//     }

    return ProcessorBase::sucess;
  }

}
#include "eudaq/Processor_merger.hh"


namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;




  RegisterProcessor(Processor_merger, "merger");



  ReturnParam Processor_merger::ProcessorEvent(ConnectionName_ref name, event_sp ev)
  {

    return ProcessorBase::sucess;
  }

  Processor_merger::Processor_merger(Parameter_ref conf) :Processor_N_x_M(conf)
  {

  }

}
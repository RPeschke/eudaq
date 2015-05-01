
#include "eudaq/Processor_parralel.hh"


namespace eudaq{

  class Processor_multi_buffer :public Processor_parallel{
  public:
    Processor_multi_buffer(Parameter_ref conf) : Processor_parallel(conf +ProConfig::Tag("ParrallelTypes",ProcessorNames::buffer())){
      
    }

  };
  using ReturnParam = ProcessorBase::ReturnParam;
  RegisterProcessor(Processor_multi_buffer, ProcessorNames::multi_buffer());
 
}
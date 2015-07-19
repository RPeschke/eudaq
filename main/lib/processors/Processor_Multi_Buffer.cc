
#include "eudaq/Processor_parralel.hh"
#include "eudaq/Processor_Buffer.hh"


namespace eudaq{

  class Processor_multi_buffer :public Processor_parallel<Proccessor_Buffer>{
  public:
    Processor_multi_buffer(Parameter_ref conf) : Processor_parallel<Proccessor_Buffer>(conf){
      
    }

  };
  using ReturnParam = ProcessorBase::ReturnParam;
 
}
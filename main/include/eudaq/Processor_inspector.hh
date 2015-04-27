#ifndef Processor_inspector_h__
#define Processor_inspector_h__

#include "eudaq/Processor.hh"



namespace eudaq{

 

  class DLLEXPORT Processor_Inspector:public Processor{
  public:


    Processor_Inspector(Parameter_ref conf);


    virtual ReturnParam inspecktEvent(const Event&)=0;


    
    ReturnParam ProcessorEvent(event_sp ev) override;
  };

  
}
#endif // Processor_inspector_h__

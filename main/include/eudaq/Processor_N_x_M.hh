#ifndef Processor_N_x_M_h__
#define Processor_N_x_M_h__

#include "eudaq/Processor_N_2_M_base.hh"

namespace eudaq{


  class Processor_N_x_M :public Processor_N_2_M_base {

  public:
    virtual ReturnParam ProcessorEvent(ConnectionName_ref name,event_sp ev) = 0;
    virtual void print(std::ostream& os);
    Processor_up CreateInterface(ConnectionName_ref name, Parameter_ref conf);

    Processor_N_x_M(Parameter_ref);
    virtual ~Processor_N_x_M(){}



    ReturnParam ProcessorEvent(event_sp ev) override;
    
    ReturnParam ProcessNext(ConnectionName_ref name, event_sp ev);
 
  };
}

#endif //Processor_N_x_M

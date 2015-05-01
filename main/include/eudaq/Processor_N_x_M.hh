#ifndef Processor_N_x_M_h__
#define Processor_N_x_M_h__

#include "eudaq/Processor_N_2_M_base.hh"

namespace eudaq{


  class Processor_N_x_M :public Processor_N_2_M_base {

  public:
    virtual std::string getName() override;
    virtual ReturnParam ProcessorEvent(ConnectionName_ref name,event_sp ev) = 0;
    virtual void print(std::ostream& os);


    Processor_N_x_M(Parameter_ref);
    virtual ~Processor_N_x_M(){}



    ReturnParam ProcessorEvent(event_sp ev) override;
    
    ReturnParam ProcessNext(ConnectionName_ref name, event_sp ev);
 
    std::map<ConnectionName_t, Processor_up> m_inputInterface;
  };
}

#endif //Processor_N_x_M

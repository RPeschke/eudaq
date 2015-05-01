#ifndef Processor_Parrallel_add2queue_h__
#define Processor_Parrallel_add2queue_h__
#include "eudaq/Processor_N_2_M_base.hh"

namespace eudaq{
  class Processor_Parrallel_add2queue :public Processor_N_2_M_base{
  public:
    Processor_Parrallel_add2queue(Parameter_ref conf);
    virtual ~Processor_Parrallel_add2queue(){}
    virtual void initialize (Configuration_ref conf);
    

    virtual ReturnParam ProcessorEvent(event_sp ev)=0;

    virtual void print(std::ostream& os) = 0;



    virtual std::string getName() ;


  
    
  private:

    class interfaceProducer;

    

    std::map<ConnectionName_t, Processor_up> m_inputInterface;
  };

}

#endif // Processor_Parrallel_add2queue_h__

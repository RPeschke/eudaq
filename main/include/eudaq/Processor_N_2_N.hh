#ifndef Processor_N_2_N_h__
#define Processor_N_2_N_h__

#include "eudaq/Processor_N_2_M_base.hh"

namespace eudaq{

  class Processor_N_2_N :public Processor_N_2_M_base{
  public:
    Processor_N_2_N(Parameter_ref conf);
    virtual ~Processor_N_2_N(){}




    virtual void multiEnd() =0;
    virtual std::string getName() = 0;
    virtual void print(std::ostream& os) = 0;





	virtual void initialize(Configuration_ref conf);
    virtual void finish() ;

    
  private:
  
    std::map<ConnectionName_t, Processor_up> m_Processors;
    virtual ReturnParam ProcessorEvent(event_sp ev) { return ProcessorBase::ret_error; }

  };

}


#endif // Processor_N_2_N_h__

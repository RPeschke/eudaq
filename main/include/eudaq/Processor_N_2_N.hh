#ifndef Processor_N_2_N_h__
#define Processor_N_2_N_h__

#include "eudaq/Processor_N_2_M_base.hh"

namespace eudaq{

  class Processor_N_2_N :public Processor_N_2_M_base{
  public:
    Processor_N_2_N(Parameter_ref conf);
    virtual ~Processor_N_2_N(){}



    virtual void Initilize() = 0;
    virtual void multiEnd() =0;
    virtual Processor_up CreateProcessor(ConnectionName_ref name, Parameter_ref conf)=0;
    virtual std::string getName() = 0;
    virtual void print(std::ostream& os) = 0;





	virtual void init(Configuration_ref conf);
    virtual void end() ;
    virtual Processor_rp getProcessor(ConnectionName_ref name = "");
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") ;
    virtual void pushProducer(Processor_up processor);
    
  private:
  
    std::map<ConnectionName_t, Processor_up> m_Processors;
    virtual ReturnParam ProcessorEvent(event_sp ev) { return ProcessorBase::ret_error; }

  };

}


#endif // Processor_N_2_N_h__

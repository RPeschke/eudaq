#ifndef Processor_N_2_N_h__
#define Processor_N_2_N_h__

#include "eudaq/ProcessorBase.hh"

namespace eudaq{

  class Processor_N_2_N :public ProcessorBase{
  public:
    Processor_N_2_N(Parameter_ref conf);
    virtual ~Processor_N_2_N(){}



    virtual void Initilize() = 0;
    virtual void multiEnd() =0;
    virtual Processor_up CreateProcessor(ConnectionName_ref name, Parameter_ref conf)=0;
    virtual std::string getName() = 0;
    virtual void print(std::ostream& os) = 0;





    virtual void init();
    virtual void end() ;
    virtual Processor_rp getProcessor(ConnectionName_ref name = "");
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") ;
    virtual void pushProducer(Processor_up processor);
    
  private:
    Processor_rp getNextProcessor(ConnectionName_t name = "");
    std::map<ConnectionName_t, Processor_up> m_Processors;
    virtual ReturnParam ProcessorEvent(event_sp ev) { return ProcessorBase::ret_error; }
    Processor_rp m_next=nullptr;
  };

}


#endif // Processor_N_2_N_h__

#ifndef Processor_Parrallel_add2queue_h__
#define Processor_Parrallel_add2queue_h__
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class Processor_Parrallel_add2queue :public ProcessorBase{
  public:
    Processor_Parrallel_add2queue(Parameter_ref conf);
    virtual ~Processor_Parrallel_add2queue(){}
    virtual void init()=0 ;
    virtual void end() = 0;

    virtual ReturnParam ProcessorEvent(event_sp ev)=0;

    virtual void print(std::ostream& os) = 0;



    virtual Processor_rp getProcessor(ConnectionName_ref name = "");
    virtual void pushProducer(Processor_up processor) {};
    virtual std::string getName() ;
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") ;

  protected:
    Processor_rp getNextProcessor(ConnectionName_ref name = "") ;
  private:
    Processor_rp m_nextProcessor=nullptr;
    class interfaceProducer;

    

    std::map<ConnectionName_t, Processor_up> m_inputInterface;
  };

}

#endif // Processor_Parrallel_add2queue_h__

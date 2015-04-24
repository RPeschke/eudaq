#ifndef Processor_splitter_h__
#define Processor_splitter_h__

#include "eudaq/ProcessorBase.hh"

namespace eudaq{


  class Processor_splitter :public ProcessorBase{

  public:
    virtual void init() override;
    virtual std::string getName() override;
    virtual ReturnParam ProcessorEvent(ConnectionName_ref name,event_sp ev) = 0;
    virtual void print(std::ostream& os);
    virtual void end()override;


    Processor_splitter(Parameter_ref);
    virtual ~Processor_splitter(){}



    ReturnParam ProcessorEvent(event_sp ev) override;
    Processor_rp getProcessor(ConnectionName_ref name = "") override;
    virtual void pushProducer(Processor_up processor){}
    void AddProcessor(Processor_rp next, ConnectionName_ref name = "") override;
    ReturnParam ProcessNext(ConnectionName_ref name, event_sp ev);
    Processor_rp m_nextProcessor = nullptr;
    std::string m_connection;
    std::map<ConnectionName_t, Processor_rp> m_nextInterfaces;


    std::map<ConnectionName_t, Processor_up> m_inputInterface;
  };
}

#endif // Processor_splitter_h__

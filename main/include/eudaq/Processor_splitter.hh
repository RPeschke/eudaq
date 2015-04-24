#ifndef Processor_splitter_h__
#define Processor_splitter_h__

#include "eudaq/ProcessorBase.hh"

namespace eudaq{


  class Processor_splitter :public ProcessorBase{

    virtual ReturnParam ProcessorEvent(event_sp ev) = 0;

    virtual std::string getName() override;
    virtual void print(std::ostream& os);


    Processor_splitter(Parameter_ref);
    virtual ~Processor_splitter(){}
    virtual void init() override;

    virtual void end()override;


    ProcessorBase* getProcessor(ConnectionName_ref name = "") override;
    virtual void pushProducer(Processor_up processor){}
    void AddProcessor(ProcessorBase* next, ConnectionName_ref name = "") override;
    ReturnParam ProcessNext(ConnectionName_ref name, event_sp ev);
    ProcessorBase* m_nextProcessor = nullptr;
    std::string m_connection;
    std::map<ConnectionName_t, ProcessorBase*> m_nextInterfaces;

  };
}

#endif // Processor_splitter_h__

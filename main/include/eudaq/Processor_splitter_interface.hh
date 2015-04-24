#ifndef Processor_splitter_interface_h__
#define Processor_splitter_interface_h__
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class Processor_splitter;
  class Processor_splitter_interface :public ProcessorBase{
  public:
    Processor_splitter_interface(Parameter_ref name);
    virtual ~Processor_splitter_interface() {};
    virtual void init(){}
    virtual ReturnParam ProcessorEvent(event_sp ev);
    virtual void end(){}



    virtual ProcessorBase* getProcessor(ConnectionName_ref name = "");

    virtual void AddProcessor(ProcessorBase *next, ConnectionName_ref name = "") ;

    virtual std::string getName();
    virtual void print(std::ostream& os);
    virtual void pushProducer(Processor_up processor) {}
  private:

    Processor_splitter* m_baseProcessor = nullptr;
    std::string m_name, m_baseName;
  };

}

#endif // Processor_splitter_interface_h__

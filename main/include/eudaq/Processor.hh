#ifndef Processor_h__
#define Processor_h__

#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class Processor:public ProcessorBase{
  public:

    
    Processor(Parameter_ref);
    virtual void init() override;
    virtual void ProcessorEvent(event_sp ev) override;
    virtual void end()override;



    virtual ProcessorBase* getProcessor(std::string name = "") override;

    virtual void AddProcessor(ProcessorBase* next, std::string = "") override;

    virtual std::string getName() override;
    virtual void print(std::ostream& os) ;

    ProcessorBase* m_next;
  };

}
#endif // Processor_h__

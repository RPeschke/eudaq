#ifndef Processor_h__
#define Processor_h__

#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class DLLEXPORT Processor:public ProcessorBase{
  public:

    
    virtual ReturnParam ProcessorEvent(event_sp ev) = 0;

    virtual std::string getName() override;
    virtual void print(std::ostream& os) ;


    Processor(Parameter_ref);
    void init() override;
 
    void end()override;

    
    ProcessorBase* getProcessor(const std::string& name = "") override;

    void AddProcessor(ProcessorBase* next,const  std::string& name = "") override;
    ReturnParam ProcessNext(event_sp ev);
    ProcessorBase* m_next=nullptr;
    std::string m_connection;
  };

}
#endif // Processor_h__

#ifndef Processor_h__
#define Processor_h__

#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class DLLEXPORT Processor:public ProcessorBase{
  public:

    
    virtual ReturnParam ProcessEvent(event_sp ev) = 0;

    virtual void print(std::ostream& os) ;
    virtual void clearProcessor() {};

    Processor(Parameter_ref);
    virtual ~Processor(){}
  virtual void init() override final;
  virtual void initialize() {}

   virtual void end()override final;
   virtual void Finish() {}
    
    Processor_rp getProcessor(const std::string& name = "") override;
    virtual void pushProcessorBase(std::unique_ptr<ProcessorBase> processor){}
    void AddProcessor(Processor_rp next, const  std::string& name = "") override;
    ReturnParam ProcessNext(event_sp ev);
    Processor_rp m_next = nullptr;
    std::string m_connection;
  };

}
#endif // Processor_h__

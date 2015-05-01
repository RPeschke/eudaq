#ifndef Processor_h__
#define Processor_h__

#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class DLLEXPORT Processor:public ProcessorBase{
  public:

    
    virtual ReturnParam ProcessorEvent(event_sp ev) = 0;

    virtual void print(std::ostream& os) ;
    virtual void clearProducer() {};

    Processor(Parameter_ref);
    virtual ~Processor(){}
	virtual void init(Configuration_ref conf) override final;
  virtual void initialize(Configuration_ref conf) {}

   virtual void end()override final;
   virtual void Finish() {}
    
    Processor_rp getProcessor(const std::string& name = "") override;
    virtual void pushProducer(std::unique_ptr<ProcessorBase> processor){}
    void AddProcessor(Processor_rp next, const  std::string& name = "") override;
    ReturnParam ProcessNext(event_sp ev);
    Processor_rp m_next = nullptr;
    std::string m_connection;
  };

}
#endif // Processor_h__

#ifndef Processor_batch_h__
#define Processor_batch_h__


#include "eudaq/Processor.hh"
#include "eudaq/Platform.hh"
#include <memory>
namespace eudaq{

  class  Processor_batch :public Processor{

  public:
    Processor_batch(Parameter_ref name);
    virtual ~Processor_batch(){}
    virtual void initialize(Configuration_ref conf) override;
    virtual ReturnParam ProcessEvent(event_sp ev) override;
    virtual void Finish()override;



    virtual ProcessorBase* getProcessor(const std::string& name = "") override;

    virtual void AddProcessor(ProcessorBase* next, const std::string& = "") override;

    
    virtual void print(std::ostream& os);
    virtual void pushProducer(std::unique_ptr<ProcessorBase> processor);

  private:
    std::vector<std::unique_ptr<ProcessorBase>> m_processors;

    ProcessorBase *m_next = nullptr;
  };
}
#endif // Processor_batch_h__
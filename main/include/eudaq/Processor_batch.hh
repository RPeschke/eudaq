#include "eudaq/Processor.hh"
namespace eudaq{

  class DLLEXPORT Processor_batch :public Processor{

  public:
    Processor_batch(Parameter_ref name);
    virtual void init() override;
    virtual ReturnParam ProcessorEvent(event_sp ev) override;
    virtual void end()override;



    virtual ProcessorBase* getProcessor(const std::string& name = "") override;

    virtual void AddProcessor(ProcessorBase* next, const std::string& = "") override;

    virtual std::string getName() override;
    virtual void print(std::ostream& os);
    void AddProcessor2Batch(std::unique_ptr<ProcessorBase> processor);

  private:
    std::vector<std::unique_ptr<ProcessorBase>> m_processors;

    ProcessorBase *m_next = nullptr;
  };
}
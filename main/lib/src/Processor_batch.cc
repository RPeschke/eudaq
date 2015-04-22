#include "eudaq/Processor.hh"
#include <memory>


namespace eudaq {
  class Processor_batch :public Processor{

  public:
    Processor_batch(Parameter_ref name);
    virtual void init() override;
    virtual void ProcessorEvent(event_sp ev) override;
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

  Processor_batch::Processor_batch(Parameter_ref name) :Processor(name)
  {

    auto splitted_names = eudaq::split(name,",");
    for (auto& e:splitted_names)
    {

      AddProcessor2Batch(ProcessorFactory::create(e, ""));
    }

  }

  void Processor_batch::init()
  {
    for (auto& e:m_processors)
    {
      e->init();
    }
  }

  void Processor_batch::end()
  {

    for (auto& e : m_processors)
    {
      e->end();
    }
  }

  ProcessorBase* Processor_batch::getProcessor(const std::string& name /*= ""*/)
  {
    return this;
  }

  std::string Processor_batch::getName()
  {
    return "batch";
  }

  void Processor_batch::AddProcessor(ProcessorBase* next,const std::string& name/*= ""*/)
  {
    m_next = next;
    m_processors.back()->AddProcessor(next, name);
  }

  void Processor_batch::print(std::ostream& os)
  {
    os << getName();
  }

  void Processor_batch::ProcessorEvent(event_sp ev)
  {
    m_processors.front()->ProcessorEvent(ev);
  }

  void Processor_batch::AddProcessor2Batch(std::unique_ptr<ProcessorBase> processor)
  {
    if (processor&&!m_processors.empty())
    {

      AddProcessor(processor.get());
    }
    m_processors.push_back(std::move(processor));
    m_next = m_processors.back().get();
  }



  RegisterProcessor(Processor_batch, "Processor_batch");
}
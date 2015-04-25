
#include "eudaq/ProcessorBase.hh"


namespace eudaq{

  class Processor_multi_buffer :public ProcessorBase{
  public:
    Processor_multi_buffer(Parameter_ref conf) : ProcessorBase(conf){
      
    }

    virtual void init() ;
    virtual ReturnParam ProcessorEvent(event_sp ev);
    virtual void end() ;



    virtual ProcessorBase* getProcessor(ConnectionName_ref name = "") ;

    virtual void AddProcessor(ProcessorBase *next, ConnectionName_ref = "");

    virtual std::string getName() = 0;
    virtual void print(std::ostream& os) = 0;
  private:
    std::map<ConnectionName_t, Processor_up> m_inputInterface;
    std::map<ConnectionName_t, Processor_rp> m_nextInterfaces;
    Processor_rp m_nextProcessor = nullptr;
    ConnectionName_t nextName;

    ProcessorBase* getProcessorInternal(ConnectionName_ref name = "");
  };
  using ReturnParam = ProcessorBase::ReturnParam;
  RegisterProcessor(Processor_multi_buffer, "multi_buffer");
  
  ReturnParam Processor_multi_buffer::ProcessorEvent(event_sp ev)
  {
    return ProcessorBase::ret_error;
  }

  void Processor_multi_buffer::init()
  {

  }

  void Processor_multi_buffer::end()
  {

  }

  ProcessorBase* Processor_multi_buffer::getProcessor(ConnectionName_ref name /*= ""*/)
  {
    auto inputItt = m_inputInterface.find(name);


    if (inputItt == m_inputInterface.end())
    {
      m_inputInterface[name] = ProcessorFactory::create("buffer", name);
      m_inputInterface[name]->AddProcessor(getProcessorInternal(name), name);

    }

    return m_inputInterface[name].get();
  }

  void Processor_multi_buffer::AddProcessor(ProcessorBase *next, ConnectionName_ref name)
  {
    if (!m_nextProcessor)
    {
      m_nextProcessor = next;
      nextName = name;
    }

    m_nextInterfaces[name] = next;
  }

  ProcessorBase* Processor_multi_buffer::getProcessorInternal(ConnectionName_ref name /*= ""*/)
  {
    auto nextItt = m_nextInterfaces.find(name);

    if (nextItt == m_nextInterfaces.end()) {
      AddProcessor(m_nextProcessor->getProcessor(name), name);
    }

    return m_nextInterfaces[name];
  }

}
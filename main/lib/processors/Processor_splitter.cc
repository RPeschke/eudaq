#include "eudaq/Processor_splitter.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  eudaq::Processor_splitter::Processor_splitter(Parameter_ref conf) :ProcessorBase(conf)
  {

  }

  void eudaq::Processor_splitter::init()
  {

  }

  void eudaq::Processor_splitter::end()
  {

  }

  void eudaq::Processor_splitter::AddProcessor(ProcessorBase* next, const ConnectionName_t& name /*= ""*/)
  {
    if (m_nextProcessor)
    {
      m_nextProcessor = next;
      m_connection = name;
    }

    m_nextInterfaces[name] = next;
  }

  ReturnParam eudaq::Processor_splitter::ProcessNext(const ConnectionName_t& name, event_sp ev)
  {
    auto nextItt = m_nextInterfaces.find(name);

    if (nextItt == m_nextInterfaces.end()) {
      AddProcessor(m_nextProcessor->getProcessor(name), name);
    }


    ProcessorBase* next_interface = m_nextInterfaces[name];
    if (next_interface)
    {
      return next_interface->ProcessorEvent(ev);
    }
    return ProcessorBase::ret_error;

  }

  ProcessorBase* Processor_splitter::getProcessor(const ConnectionName_t& name /*= ""*/)
  {

  }

  std::string Processor_splitter::getName()
  {
    return "splitter";
  }

  void Processor_splitter::print(std::ostream& os)
  {
    os << getName();
  }

}
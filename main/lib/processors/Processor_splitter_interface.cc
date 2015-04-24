#include "eudaq/Processor_splitter_interface.hh"
#include "eudaq/Processor_splitter.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  ProcessorBase* Processor_splitter_interface::getProcessor(ConnectionName_ref name /*= ""*/)
  {
    m_name = name;
    return this;

  }

  void Processor_splitter_interface::AddProcessor(ProcessorBase *next, ConnectionName_ref name /*= ""*/)
  {

    Processor_splitter* base = dynamic_cast<Processor_splitter*>(next);
    if (base)
    {
      m_baseName = name;
      m_baseProcessor = base;
    }
  }

  std::string Processor_splitter_interface::getName()
  {
    return m_name;
  }

  void Processor_splitter_interface::print(std::ostream& os)
  {
    std::cout << getName();
  }

  ReturnParam Processor_splitter_interface::ProcessorEvent(event_sp ev)
  {
    return m_baseProcessor->ProcessorEvent(getName(), ev);
  }

  Processor_splitter_interface::Processor_splitter_interface(Parameter_ref conf) :ProcessorBase(conf)
  {

  }





}


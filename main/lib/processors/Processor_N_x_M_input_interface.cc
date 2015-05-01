#include "eudaq/Processor_N_x_M_input_interface.hh"
#include "eudaq/Processor_N_x_M.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  ProcessorBase* Processor_N_x_M_input_interface::getProcessor(ConnectionName_ref name /*= ""*/)
  {
    m_name = name;
    return this;

  }

  void Processor_N_x_M_input_interface::AddProcessor(ProcessorBase *next, ConnectionName_ref name /*= ""*/)
  {

    Processor_N_x_M* base = dynamic_cast<Processor_N_x_M*>(next);
    if (base&&!m_baseProcessor)
    {
      m_baseName = name;
      m_baseProcessor = base;
    }
  }

  std::string Processor_N_x_M_input_interface::getName()
  {
    return m_name;
  }

  void Processor_N_x_M_input_interface::print(std::ostream& os)
  {
    std::cout << getName();
  }

  ReturnParam Processor_N_x_M_input_interface::ProcessorEvent(event_sp ev)
  {
    return m_baseProcessor->ProcessorEvent(getName(), ev);
  }

  Processor_N_x_M_input_interface::Processor_N_x_M_input_interface(Parameter_ref conf) :ProcessorBase(conf)
  {

  }





}


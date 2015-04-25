#include "eudaq/Processor_N_x_M.hh"
#include "eudaq/Processor_N_x_M_input_interface.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
   Processor_N_x_M::Processor_N_x_M(Parameter_ref conf) :ProcessorBase(conf)
  {

  }

  void Processor_N_x_M::init()
  {

  }

  void  Processor_N_x_M::end()
  {

  }



  ReturnParam Processor_N_x_M::ProcessNext(const ConnectionName_t& name, event_sp ev)
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

  ProcessorBase* Processor_N_x_M::getProcessor(const ConnectionName_t& name /*= ""*/)
  {
    auto inputItt = m_inputInterface.find(name);


    if (inputItt==m_inputInterface.end())
    {
      m_inputInterface[name] = Processor_up(new Processor_N_x_M_input_interface( name ));
      m_inputInterface[name]->AddProcessor(this, "base");

    }

    return m_inputInterface[name].get();
  }

  std::string Processor_N_x_M::getName()
  {
    return "splitter";
  }

  void Processor_N_x_M::print(std::ostream& os)
  {
    os << getName();
  }

  ReturnParam Processor_N_x_M::ProcessorEvent(event_sp ev)
  {
   return ProcessorEvent(getName(), ev);
  }

  void Processor_N_x_M::AddProcessor(Processor_rp next, ConnectionName_ref name /*= ""*/)
  {
    if (!m_nextProcessor)
    {
      m_nextProcessor = next;
      m_connection = name;
    }

    m_nextInterfaces[name] = next;
  }




}
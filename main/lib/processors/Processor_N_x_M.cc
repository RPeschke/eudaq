#include "eudaq/Processor_N_x_M.hh"
#include "eudaq/Processor_N_x_M_input_interface.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
   Processor_N_x_M::Processor_N_x_M(Parameter_ref conf) :Processor_N_2_M_base(conf)
  {

  }





  ReturnParam Processor_N_x_M::ProcessNext(const ConnectionName_t& name, event_sp ev)
  {
    auto next = getNextProcessor(name);
    if (next)
    {
      return next->ProcessorEvent(std::move(ev));
    }
    
    return ProcessorBase::ret_error;

  }



  std::string Processor_N_x_M::getName()
  {
    return "splitter";
  }

  void Processor_N_x_M::print(std::ostream& os)
  {
    os << getName();
  }

  Processor_up Processor_N_x_M::CreateInterface(ConnectionName_ref name, Parameter_ref conf)
  {

  }

  ReturnParam Processor_N_x_M::ProcessorEvent(event_sp ev)
  {
   return ProcessorEvent(getName(), ev);
  }






}
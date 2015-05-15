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
      return next->ProcessEvent(std::move(ev));
    }
    
    return ProcessorBase::ret_error;

  }




  void Processor_N_x_M::print(std::ostream& os)
  {
    os << getName();
  }

  Processor_up Processor_N_x_M::CreateInterface(ConnectionName_ref name, Parameter_ref conf)
  {
    auto processor=Processor_up(new Processor_N_x_M_input_interface(name));
    
    Processor_N_x_M_input_interface* dummy_interface = dynamic_cast<Processor_N_x_M_input_interface*> (processor.get());
    if (dummy_interface)
    {
      dummy_interface->AddProcessor(this, "base");

    }
    return processor;
  }

  ReturnParam Processor_N_x_M::ProcessEvent(event_sp ev)
  {
   return ProcessEvent(getName(), ev);
  }






}
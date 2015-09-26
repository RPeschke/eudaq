#include "eudaq/Processor_inspector.hh"




namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;


  

  ReturnParam Processor_Inspector::ProcessEvent(event_sp ev)
  {
    if (!ev)
    {
      return stop;
    }
    auto ret = inspecktEvent(*ev);

    if (ret!=sucess)
    {
      return ret;
    }

   return ProcessNext(std::move(ev));
  }

  Processor_Inspector::Processor_Inspector(Parameter_ref conf) :Processor(conf)
  {

  }

  Processor_Inspector::Processor_Inspector() : Processor(Parameter_t("DAS")) {

  }

}


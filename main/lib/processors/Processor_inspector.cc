#include "eudaq/Processor_inspector.hh"




namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;




  ReturnParam Processor_Inspector::ProcessEvent(event_sp ev, ConnectionName_ref con) {
    if (!ev) {
      return stop;
    }
    auto ret = inspecktEvent(*ev,con);

    if (ret != sucess) {
      return ret;
    }

    return processNext(std::move(ev),con);
  }

  Processor_Inspector::Processor_Inspector(Parameter_ref conf) :ProcessorBase(conf)
  {

  }

  Processor_Inspector::Processor_Inspector() : ProcessorBase(Parameter_t("unnamed")) {

  }

}


#include "eudaq/ProcessorBase.hh"
#include "eudaq/factoryDef.hh"
#include "eudaq/OptionParser.hh"

namespace eudaq{
  


ProcessorBase::ProcessorBase(ProcessorBase::Parameter_ref name):m_conf(name) {

}

void ProcessorBase::AddNextProcessor(Processor_rp next) {
    m_next = next;
  }


 




ProcessorBase::ReturnParam ProcessorBase::processNext(event_sp ev, ConnectionName_ref con) {
  if (m_next)
  {
    return m_next->ProcessEvent(std::move(ev), con);
  }
  return sucess;
}

}

#include "eudaq/Processor_inspector.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Processor_eventSelectors.hh"

namespace eudaq{
 

select_events::select_events(const std::vector<unsigned>& eventsOfIntresst, bool doBore /*= true*/, bool doEORE /*= true*/) :Processor_Inspector(ProcessorBase::Parameter_t("")), m_eventsOfInterest(eventsOfIntresst), m_do_bore(doBore), m_do_eore(doEORE) {

}

ProcessorBase::ReturnParam select_events::inspecktEvent(const Event&ev, ConnectionName_ref con) {
  if (ev.IsBORE() && m_do_bore) {
    return ProcessorBase::sucess;
  }

  if (ev.IsEORE() && m_do_eore) {
    return ProcessorBase::sucess;
  }
  if (m_eventsOfInterest.empty()) {
    return ProcessorBase::sucess;
  }

  if (ev.GetEventNumber() > m_eventsOfInterest.back()) {
    return ProcessorBase::stop;
  }


  if (std::find(m_eventsOfInterest.begin(), m_eventsOfInterest.end(),
    ev.GetEventNumber()) != m_eventsOfInterest.end()) {
    return ProcessorBase::sucess;
  }
  return ProcessorBase::skip_event;
}

}
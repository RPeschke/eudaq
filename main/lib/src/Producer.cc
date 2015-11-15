#include "eudaq/TransportClient.hh"
#include "eudaq/Producer.hh"
#include "eudaq/Processor_inspector.hh"
#include "eudaq/Processors.hh"
#include <memory>
#include "eudaq/Processor_batch.hh"

namespace eudaq {

  Producer::Producer(const std::string & name, const std::string & runcontrol)
    : CommandReceiver("Producer", name, runcontrol), m_name(name)
  {
  }

  void Producer::OnData(const std::string & param) {
    
    m_dataSender =  Processors::dataSender(param, "Producer", m_name);

  }

  void Producer::SendEvent(const Event & ev) {
    m_dataSender->inspectEvent(ev, 0);
  }

}

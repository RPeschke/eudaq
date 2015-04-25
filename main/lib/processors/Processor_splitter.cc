#include "eudaq/Processor_splitter.hh"

#include "eudaq/PluginManager.hh"
#include "eudaq/DetectorEvent.hh"
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  ReturnParam eudaq::Processor_splitter::ProcessorEvent(ConnectionName_ref name, event_sp ev)
  {

    if (ev->IsPacket())
    {
      for (size_t i = 0; i < PluginManager::GetNumberOfROF(*ev);++i)
      {
        ProcessorBaseEvent(name, PluginManager::ExtractEventN(ev, i));
      }
      if (ev->get_id()==DetectorEvent::eudaq_static_id())
      {
        auto det = dynamic_cast<DetectorEvent*>(ev.get());
        det->clearEvents();
        ProcessorBaseEvent(name, ev);
      }
    }
    
    ProcessorBaseEvent(name, ev);

  }

  ReturnParam Processor_splitter::ProcessorBaseEvent(ConnectionName_ref name, event_sp ev)
  {

    auto id=PluginManager::getUniqueIdentifier(*ev);



  }

}
#include "eudaq/aidaDataConverterPlugin.hh"
#include "eudaq/Event.hh"
#include "eudaq/TLUEvent.hh"
#include "eudaq/resyncUtilities.hh"

namespace eudaq{

int aidaDataConverterPlugin::IsSyncWithTLU(AidaPacket const & ev,TLUEvent const & tlu ) const
{
  // dummy comparator. it is just checking if the event numbers are the same.

  //auto triggerID=ev.GetEventNumber();
  unsigned triggerID=ev.GetTag<unsigned>("tlu_trigger_id",0);
  auto tlu_triggerID=tlu.GetEventNumber();
  return compareTLU2DUT(tlu_triggerID,triggerID);
}

void aidaDataConverterPlugin::setCurrentTLUEvent( AidaPacket & ev,TLUEvent const & tlu )
{
  ev.SetTag("tlu_trigger_id",tlu.GetEventNumber());
}



unsigned aidaDataConverterPlugin::GetTriggerID( AidaPacket const & ai) const
{
  
}

unsigned aidaDataConverterPlugin::GetTriggerID(AidaPacket const& ai, unsigned const TriggerNo) const{


}
}
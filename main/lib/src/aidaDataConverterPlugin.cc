#include "eudaq/aidaDataConverterPlugin.hh"
#include "eudaq/Event.hh"
#include "eudaq/TLUEvent.hh"
#include "eudaq/resyncUtilities.hh"
#include "eudaq/AidaPacket.hh"

namespace eudaq{

int aidaDataConverterPlugin::IsSyncWithTLU(AidaPacket const & ev,TLUEvent const & tlu ) const
{
  // dummy comparator. it is just checking if the event numbers are the same.

  auto triggerID=ev.GetPacketNumber();
  auto tlu_triggerID=tlu.GetEventNumber();
  return compareTLU2DUT(tlu_triggerID,triggerID);
}

void aidaDataConverterPlugin::setCurrentTLUEvent( AidaPacket & ev,TLUEvent const & tlu )
{
  ev.SetTag("tlu_trigger_id",tlu.GetEventNumber());
}



unsigned aidaDataConverterPlugin::GetTriggerID( AidaPacket const & ai) const
{
  return ai.GetMetaData().getTriggerID(0);
}

unsigned aidaDataConverterPlugin::GetTriggerID(AidaPacket const& ai, unsigned const TriggerNo) const{

  return ai.GetMetaData().getTriggerID(TriggerNo);
}

bool aidaDataConverterPlugin::GetStandardSubEvent( StandardEvent & /*result*/, AidaPacket const & /*source*/ ) const
{
  return false;
}

aidaDataConverterPlugin::aidaDataConverterPlugin( std::string subtype )
{
  uint64_t type=AidaPacket::str2type("_raw");
  uint64_t subt=AidaPacket::str2type(subtype);
  m_eventtype=std::make_pair(type,subt);
}

aidaDataConverterPlugin::aidaDataConverterPlugin( std::string type, std::string subtype  )
{
  uint64_t maintype=AidaPacket::str2type(type);
  uint64_t subt=AidaPacket::str2type(subtype);
  m_eventtype=std::make_pair(maintype,subt);
}

}
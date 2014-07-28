#include "eudaq/aidaPluginManager.hh"
#include "eudaq/aidaDataConverterPlugin.hh"


AidaPluginManager& eudaq::AidaPluginManager::GetInstance()
{
  // the only one static instance of the plugin manager is in the getInstance function
  // like this it is ensured that the instance is created before it is used
  static AidaPluginManager manager;
  return manager;
}

unsigned eudaq::AidaPluginManager::GetTriggerID( const AidaPacket & ev)
{
   return GetInstance().GetPlugin(ev).GetTriggerID(ev);
}

eudaq::AidaPluginManager::t_packetid eudaq::AidaPluginManager::getEventId( eudaq::AidaPacket const & ev )
{

}

void eudaq::AidaPluginManager::RegisterPlugin( aidaDataConverterPlugin * plugin )
{
  m_pluginmap[plugin->GetEventType()] = plugin;
}

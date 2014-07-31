#include "eudaq/aidaPluginManager.hh"
#include "eudaq/aidaDataConverterPlugin.hh"

namespace eudaq{
  AidaPluginManager& AidaPluginManager::GetInstance()
  {
    // the only one static instance of the plugin manager is in the getInstance function
    // like this it is ensured that the instance is created before it is used
    static AidaPluginManager manager;
    return manager;
  }

  unsigned AidaPluginManager::GetTriggerID(const AidaPacket & ev)
  {
    return GetInstance().GetPlugin(ev).GetTriggerID(ev);
  }

  AidaPluginManager::t_packetid AidaPluginManager::getEventId(AidaPacket const & ev)
  {
    return GetInstance().GetPlugin(ev).GetEventType();
  }

  void AidaPluginManager::RegisterPlugin(aidaDataConverterPlugin * plugin)
  {
    m_pluginmap[plugin->GetEventType()] = plugin;
  }

  aidaDataConverterPlugin & AidaPluginManager::GetPlugin(t_packetid eventtype)
  {
    auto pluginiter = m_pluginmap.find(eventtype);
    
    if (pluginiter == m_pluginmap.end()) {
      EUDAQ_THROW("PluginManager::GetPlugin(): Unkown event type " + AidaPacket::type2str(eventtype.first) + ":" + AidaPacket::type2str(eventtype.second));
    }

    return *pluginiter->second;
  }

  aidaDataConverterPlugin & AidaPluginManager::GetPlugin(const AidaPacket & event)
  {
    t_packetid pid(event.GetPacketType(), event.GetPacketSubType());
    return GetPlugin(pid);
  }

}
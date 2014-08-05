#ifndef  aidaPluginManager_hh__
#define  aidaPluginManager_hh__
#include "Platform.hh"
#include "AidaPacket.hh"


namespace lcio{

  class LCRunHeader;
  class LCEvent;

}

namespace eudaq {
  class aidaDataConverterPlugin;
  class StandardEvent;
  class TLUPacket;


  /** The plugin manager has a map of all available plugins.
  *  On creating time every plugin automatically registers at
  *  the plugin manager, which adds the event type string and
  *  a pointer to the according plugin to a map.
  *  A generic event being received contains an identification string,
  *  and the plugin manager can deliver the correct plugin to
  *  convert it to lcio.
  */
  class  AidaPluginManager {

  public:
    typedef std::pair<uint64_t,uint64_t> t_packetid;

    /** Register a new plugin to the plugin manager.
    */
    void RegisterPlugin(aidaDataConverterPlugin * plugin);

    /** Get the instance of the plugin manager. As this is a singleton class with
    *  private constructor and copy constructor, this is the only way to access it.
    */
    static AidaPluginManager & GetInstance();

    static unsigned GetTriggerID(const AidaPacket &);

    static int IsSyncWithTLU(eudaq::AidaPacket const & ev, TLUPacket const & tlu);
    static t_packetid getEventId(eudaq::AidaPacket const & ev);

    static void setCurrentTLUEvent(eudaq::AidaPacket & ev, TLUPacket const & tlu);
    static void Initialize(const  AidaPacket &);
    static lcio::LCRunHeader * GetLCRunHeader(const  AidaPacket &);
    static StandardEvent ConvertToStandard(const  AidaPacket &);
    static lcio::LCEvent * ConvertToLCIO(const  AidaPacket &);

    static void ConvertStandardSubEvent(StandardEvent &, const AidaPacket &);
    static void ConvertLCIOSubEvent(lcio::LCEvent &, const AidaPacket &);

    /** Get the correct plugin implementation according to the event type.
    */
    aidaDataConverterPlugin & GetPlugin(t_packetid eventtype);
    aidaDataConverterPlugin & GetPlugin(const AidaPacket & event);

  private:
    /** The map that correlates the event type with its converter plugin.
    */
    std::map<t_packetid, aidaDataConverterPlugin*> m_pluginmap;

    AidaPluginManager() {}
    AidaPluginManager(AidaPluginManager const &) {}
    class _dummy;
    friend class _dummy; // Silence superfluous warnings in some gcc versions
  };

}//namespace eudaq

#endif // !aidaPluginManager_hh__

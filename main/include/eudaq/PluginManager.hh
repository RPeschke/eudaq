#ifndef EUDAQ_INCLUDED_PluginManager
#define EUDAQ_INCLUDED_PluginManager

#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/genericDetectorContainer.hh"


#include <string>
#include <map>
#include "Configuration.hh"

namespace eudaq {

  /** The plugin manager has a map of all available plugins.
   *  On creating time every plugin automatically registers at
   *  the plugin manager, which adds the event type string and 
   *  a pointer to the according plugin to a map.
   *  A generic event being received contains an identification string,
   *  and the plugin manager can deliver the correct plugin to 
   *  convert it to lcio.
   */
  template <typename containerT>
  class DLLEXPORT  PluginManager {

    public:
      typedef typename containerT::t_id t_eventid;
      typedef genericDetContainer<containerT>  detContainer;
      /** Register a new plugin to the plugin manager.
       */
      void RegisterPlugin(DataConverterPlugin<containerT> * plugin);

      /** Get the instance of the plugin manager. As this is a singleton class with
       *  private constructor and copy constructor, this is the only way to access it.
       */
      static PluginManager<containerT> & GetInstance();

      static unsigned GetTriggerID(const containerT &ev);

      static int IsSyncWithTLU(const containerT & ev, const containerT & tlu);
      static t_eventid getEventId(const containerT & ev){
        return GetInstance().GetPlugin(ev).GetEventType();
      }

      static void setCurrentTLUEvent(containerT & ev, const containerT & tlu);
    template <typename detEv>
    static void Initialize(const detEv &dev){
      eudaq::Configuration conf(dev.GetTag("CONFIG"));
      conf.Set("timeDelay", dev.GetTag("longTimeDelay", "0"));
      for (size_t i = 0; i < dev.NumEvents(); ++i) {
        const containerT & subev = *dev.GetEvent(i);
        GetInstance().GetPlugin(subev).Initialize(subev, conf);
      }
    }
//      static lcio::LCRunHeader * GetLCRunHeader(const detContainer &);
      template <typename detEv>
      static StandardEvent ConvertToStandard(const detEv &dev){
        //StandardEvent event(dev.GetRunNumber(), dev.GetEventNumber(), dev.GetTimestamp());
        StandardEvent event(dev);
        for (size_t i = 0; i < dev.NumEvents(); ++i) {
          const Event * ev = dev.GetEvent(i);
          if (!ev) EUDAQ_THROW("Null event!");
          if (ev->GetSubType() == "EUDRB") {
            ConvertStandardSubEvent(event, *ev);
          }
        }
        for (size_t i = 0; i < dev.NumEvents(); ++i) {
          const Event * ev = dev.GetEvent(i);
          if (!ev) EUDAQ_THROW("Null event!");
          if (ev->GetSubType() != "EUDRB") {
            ConvertStandardSubEvent(event, *ev);
          }
        }
        return event;
      }


      static void ConvertStandardSubEvent(StandardEvent &dest, const containerT &source);

      static size_t getNumberOfElemts(const containerT &ev);
      static bool   getElement(containerT &source, size_t elementNr);

      /** Get the correct plugin implementation according to the event type.
       */
      DataConverterPlugin<containerT> & GetPlugin(t_eventid eventtype);


      DataConverterPlugin<containerT> & GetPlugin(const containerT & ev);


    private:
      /** The map that correlates the event type with its converter plugin.
       */
      std::map<t_eventid, DataConverterPlugin<containerT> *> m_pluginmap;

      PluginManager() {}
      PluginManager(PluginManager const &) {}
      class _dummy;
      friend class _dummy; // Silence superfluous warnings in some gcc versions
  };



}//namespace eudaq

#endif //EUDAQ_INCLUDED_PluginManager

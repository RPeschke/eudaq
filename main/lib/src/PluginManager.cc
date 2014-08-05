#include "eudaq/PluginManager.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Configuration.hh"

#if USE_LCIO
#  include "lcio.h"
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/LCRunHeaderImpl.h"
#endif

#if USE_EUTELESCOPE
#  include "EUTELESCOPE.h"
#  include "EUTelRunHeaderImpl.h"
using eutelescope::EUTELESCOPE;
#endif

//#include <iostream>
#include <string>

#define registerNewPluginManagerType(type) void temp(type& t){\
                                              TemporaryFunction<type>(t);\
                                            } \
                                            const int dummy=0

using namespace std;

namespace eudaq {
  template <typename containerT>
  PluginManager<containerT> & PluginManager<containerT>::GetInstance() {
    // the only one static instance of the plugin manager is in the getInstance function
    // like this it is ensured that the instance is created before it is used
    static PluginManager<containerT> manager;
    return manager;
  }

  template <typename containerT>
  DataConverterPlugin<containerT> & PluginManager<containerT>::GetPlugin(const containerT & ev) {
    return GetPlugin(ev.getID());
  }
  template <typename containerT>
  DataConverterPlugin<containerT> & PluginManager<containerT>::GetPlugin(t_eventid eventtype) {
    auto pluginiter = m_pluginmap.find(eventtype);

    if (pluginiter == m_pluginmap.end()) {
      EUDAQ_THROW("PluginManager::GetPlugin(): Unkown event type "+to_string(eventtype));
    }

    return *pluginiter->second;
  }




#if USE_LCIO && USE_EUTELESCOPE
  lcio::LCRunHeader * PluginManager<Event>::GetLCRunHeader(const DetectorEvent & bore) {
    IMPL::LCRunHeaderImpl * lcHeader = new IMPL::LCRunHeaderImpl;
    lcHeader->setRunNumber(bore.GetRunNumber());
    lcHeader->setDetectorName("EUTelescope");
    eutelescope::EUTelRunHeaderImpl runHeader(lcHeader);
    runHeader.setDateTime();
    runHeader.setDataType(EUTELESCOPE::DAQDATA);
    runHeader.setDAQSWName(EUTELESCOPE::EUDAQ);

    const eudaq::Configuration conf(bore.GetTag("CONFIG"));
    runHeader.setGeoID(conf.Get("GeoID", 0));

    for (size_t i = 0; i < bore.NumEvents(); ++i) {
      const eudaq::Event & subev = *bore.GetEvent(i);
      GetInstance().GetPlugin(subev).GetLCIORunHeader(*lcHeader, subev, conf);
    }
    return lcHeader;
  }
#else
//   template <typename containerT>
//   lcio::LCRunHeader * PluginManager<containerT>::GetLCRunHeader(const detContainer &) {
//     return 0;
//   }
#endif


#if USE_LCIO
  lcio::LCEvent * PluginManager<Event>::ConvertToLCIO(const DetectorEvent & dev) {
    lcio::LCEventImpl * event = new lcio::LCEventImpl;
    event->setEventNumber(dev.GetEventNumber());
    event->setRunNumber(dev.GetRunNumber());
    event->setTimeStamp(dev.GetTimestamp());

    for (size_t i = 0; i < dev.NumEvents(); ++i) {
      ConvertLCIOSubEvent(*event, *dev.GetEvent(i));
    }

    return event;
  }
#else
//   template <typename containerT>
//   lcio::LCEvent * PluginManager<containerT>::ConvertToLCIO(const detContainer &) {
//     return 0;
//   }
#endif





template <typename containerT>
unsigned PluginManager<containerT>::GetTriggerID(const containerT &ev)
{
  return GetInstance().GetPlugin(ev).GetTriggerID(ev);
}


template <typename containerT>
int PluginManager<containerT>::IsSyncWithTLU(const containerT & ev, const containerT & tlu)
{
  return GetInstance().GetPlugin(ev).IsSyncWithTLU(ev, tlu);
}





template <typename containerT>
void PluginManager<containerT>::setCurrentTLUEvent(containerT & ev, const containerT & tlu)
{
  GetInstance().GetPlugin(ev).setCurrentTLUEvent(ev, tlu);
}


template <typename containerT>
void PluginManager<containerT>::ConvertStandardSubEvent(StandardEvent &dest, const containerT &source)
{
  try {
    GetInstance().GetPlugin(source).GetStandardSubEvent(dest, source);
  }
  catch (const Exception & e) {
    std::cerr << "Error during conversion in PluginManager::ConvertStandardSubEvent:\n" << e.what() << std::endl;
  }
}





template <typename containerT>
void PluginManager<containerT>::RegisterPlugin(DataConverterPlugin<containerT> * plugin)
{
  m_pluginmap[plugin->GetEventType()] = plugin;
}

template <typename containerT>
size_t eudaq::PluginManager<containerT>::getNumberOfElemts(const containerT &ev)
{
  return GetInstance().GetPlugin(ev).getNumberOfElemts(ev);
}

template <typename containerT>
bool eudaq::PluginManager<containerT>::getElement(containerT &ev, size_t elementNr)
{
  return GetInstance().GetPlugin(ev).getElement(ev, elementNr);
}

}//namespace eudaq


// No need to call this TemporaryFunction() function,
// it's just to avoid link error.
template <typename T>
void TemporaryFunction(T& t)
{  
  eudaq::PluginManager < T >::GetInstance();
  eudaq::PluginManager < T >::GetInstance().GetPlugin(t);
  eudaq::PluginManager < T >::GetInstance().GetTriggerID(t);
  eudaq::PluginManager < T >::GetInstance().IsSyncWithTLU(t, t);
  eudaq::PluginManager < T >::GetInstance().setCurrentTLUEvent(t, t);
  eudaq::PluginManager < T >::GetInstance().ConvertStandardSubEvent(eudaq::StandardEvent(), t);
  eudaq::DataConverterPlugin<T> * plugin = nullptr;
  eudaq::PluginManager < T >::GetInstance().RegisterPlugin(plugin);
}

#ifndef EUDAQ_INCLUDED_DataConverterPlugin
#define EUDAQ_INCLUDED_DataConverterPlugin

#include <string>
#include <algorithm>

#include "eudaq/StandardEvent.hh"
#include "eudaq/resyncUtilities.hh"

#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/TrackerDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "IMPL/LCGenericObjectImpl.h"
#  include "UTIL/CellIDEncoder.h"
#  include "lcio.h"
using namespace IMPL;
using namespace UTIL;
#endif
//#include "Configuration.hh"





#define NOTIMESTAMPSET (uint64_t)-1
#define NOTIMEDURATIONSET 0









namespace EVENT { class LCEvent; class LCRunHeader; }
namespace lcio { using namespace EVENT; }



namespace eudaq{



  class Configuration;

  /**
   *  The DataConverterPlugin is the abstract base for all plugins. The
   *  actual impementation provides the GetLCIOEvent() and GetStandardEvent()
   *  functions which
   *  get the eudaq::Event as input parameter.
   *
   *  The implementations should be singleton classes which only can be
   *  accessed via the plugin manager. (See TimepixConverterPlugin as example).
   *  The plugin implementations have to register with the plugin manager.
   */
  template <typename containerT>
  class DataConverterPlugin {
  public:
    typedef typename containerT::t_id     t_eventid;
    typedef typename containerT::mainType mainType;
    typedef typename containerT::subType  subType;

    virtual void Initialize(containerT const &, Configuration const &) {}

    virtual unsigned GetTriggerID(containerT const &ev) const{ return ev.GetEventNumber(); }
    virtual int IsSyncWithTLU(containerT const & ev, containerT const & tlu) const {
      // dummy comparator. it is just checking if the event numbers are the same.

      //auto triggerID=ev.GetEventNumber();
      unsigned triggerID = ev.GetTag<unsigned>("tlu_trigger_id", 0);
      auto tlu_triggerID = tlu.GetEventNumber();
      return compareTLU2DUT(tlu_triggerID, triggerID);
    }

    virtual void setCurrentTLUEvent(containerT & ev, containerT const & tlu){
      ev.SetTag("tlu_trigger_id", tlu.GetEventNumber());
    }
    virtual void GetLCIORunHeader(lcio::LCRunHeader &, containerT const &, Configuration const &) const {}


    /** Returns the LCIO version of the event.
     */
    virtual bool GetLCIOSubEvent(lcio::LCEvent & /*result*/, containerT const & /*source*/) const { return false; }

    /** Returns the StandardEvent version of the event.
     */
    virtual bool GetStandardSubEvent(StandardEvent & /*result*/, containerT const & /*source*/) const { return false; };

    /** Returns the type of event this plugin can convert to lcio as a pair of Event type id and subtype string.
     */
    virtual t_eventid const & GetEventType() const { return m_eventtype; }


    // the default is that one container contains only one event. if it changes the user has to overload this function in their plugin 
    virtual size_t getNumberOfElemts(const containerT &ev){ return 1; } 
    virtual void  getElement(const containerT &ev, size_t elementNr){ 
      
      if ((elementNr >= getNumberOfElemts(ev)))
      {
        EUDAQ_THROW("Trying to access an element out of range");
      }
      m_elenentNumerator = elementNr;
      
    }

    virtual size_t getCurrentNumerator(const containerT &ev) { return m_elenentNumerator; }

    virtual bool NextNumerator(const containerT &ev){
      if (getCurrentNumerator(ev)+1 < getNumberOfElemts(ev)){
        getElement(ev,getCurrentNumerator(ev) + 1);
        return true;
      }
      return false;
    }

    /** The empty destructor. Need to add it to make it virtual.
     */
    virtual ~DataConverterPlugin() {}


    /** The constructor which automatically registers the plugin
    *  at the pluginManager.
    */
    DataConverterPlugin(subType subtype);

    DataConverterPlugin(mainType type, subType subtype);
  protected:
    /** The string storing the event type this plugin can convert to lcio.
     *  This string has to be set in the constructor of the actual implementations
     *  of the plugin.
     */
    t_eventid m_eventtype;
    size_t m_elenentNumerator=0;



  private:
    /** The private copy constructor and assignment operator. They are not used anywhere, so there is not
     *  even an implementation. Even if the childes default copy constructor is public
     *  the code will not compile if it is called, since it cannot access this cc, which the
     *  the default cc does.
     */
    DataConverterPlugin(DataConverterPlugin &);
    DataConverterPlugin & operator = (const DataConverterPlugin &);
  };





}//namespace eudaq

#endif // EUDAQ_INCLUDED_DataConverterPlugin

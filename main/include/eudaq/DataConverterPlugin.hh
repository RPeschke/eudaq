#ifndef EUDAQ_INCLUDED_DataConverterPlugin
#define EUDAQ_INCLUDED_DataConverterPlugin

#include <memory>
#include "eudaq/StandardEvent.hh"
#include "eudaq/RawDataEvent.hh"



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
#include "TLUEvent.hh"

#define NOTIMESTAMPSET (uint64_t)-1
#define NOTIMEDURATIONSET 0



//////////////////////////////////////////////////////////////////////////
// Compare Time stamps

// DUT_Trigger_number > TLU_Trigger_Number
#define Event_IS_EARLY -1              

// TLU_Trigger_Number > DUT_Trigger_number
#define Event_IS_LATE 1

// DUT_Trigger_number == TLU_Trigger_Number
#define Event_IS_Sync 0





namespace EVENT { class LCEvent; class LCRunHeader; }
namespace lcio { using namespace EVENT; }

#include <string>
#include <algorithm>

namespace eudaq{


	  inline int compareTLU2DUT(unsigned TLU_Trigger_Number, unsigned DUT_Trigger_number){
	  if (DUT_Trigger_number==TLU_Trigger_Number)
	  {
		  return Event_IS_Sync;	
	  }else if (DUT_Trigger_number>TLU_Trigger_Number)
	  {
		  return Event_IS_EARLY;
	  }
	  return Event_IS_LATE;



  }
  template <typename T>
  inline int compareTLU2DUT(T TLU_Trigger_Number, T DUT_Trigger_number){
	  if (DUT_Trigger_number==TLU_Trigger_Number)
	  {
		  return Event_IS_Sync;	
	  }else if (DUT_Trigger_number>TLU_Trigger_Number)
	  {
		  return Event_IS_EARLY;
	  }
	  return Event_IS_LATE;



  }

  template <typename T>
  inline int hasTimeOVerlaping(T eventBegin, T EventEnd, T TLUStart,T TLUEnd){



	  if (TLUStart <= EventEnd
					&&
		  eventBegin <= TLUEnd){

		  /*

						  | event start  |event End
		  ----------------------------------------->
		  t

				  | tlu start  | tlu End
		  ------------------------------------------>
		  t

		  */

		  return Event_IS_Sync;

	  }
	  else if (EventEnd<TLUStart){
		  /*

		  | event start  |event End
		  ----------------------------------------->
												  t

								  | tlu start  | tlu End
		  ------------------------------------------>
												  t

		  */
		  return Event_IS_LATE;
	  }else if (eventBegin>TLUEnd)
	  {

		  /*

								| event start  |event End
		  ----------------------------------------->
												t

		  | tlu start  | tlu End
		  ------------------------------------------>
												t

		  */
		  return Event_IS_EARLY;

	  }
		  
	  EUDAQ_THROW("unforseen case this shold not happen");

	  return -666;
  }

  class CompareTimeStampsWithJitter{
  public:
    CompareTimeStampsWithJitter(uint64_t jitter_denominator, uint64_t jitter_offset, uint64_t default_delta_timestamps,uint64_t DUT_active_time, bool use_sync_events) :
      m_jitter_denominator(jitter_denominator), 
      m_jitter_offset(jitter_offset), 
      m_default_delta_timestamps(default_delta_timestamps),
      m_DUT_active_time(DUT_active_time),
      m_useSyncEvents(use_sync_events)
    {
    }

    bool isSyncEvent(const eudaq::Event  & tluEvent) const{

      return tluEvent.GetTag("trigger", 0) == 1;
    }

    int compareDUT2TLU(eudaq::Event const & ev, const eudaq::Event  & tluEvent)const{

      if (m_useSyncEvents)
      {

        if (isSyncEvent(tluEvent))
        {
          return compareDUT2TLU_sync_event(ev, tluEvent);
        }
        else
        {
          return compareDUT2TLU_normal_Event(ev, tluEvent);
        }


      }

      auto sync = compareDUT2TLU_normal_Event(ev, tluEvent);
      if (sync==Event_IS_Sync)
      {
        resync(ev, tluEvent);
      }
      return sync;

    }

    void resync(eudaq::Event const & ev,const eudaq::Event  & tluEvent) const{
    
      auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;
      m_last_tlu = TLU_TimeStamp;
      m_dut_begin = ev.GetTimestamp() + m_default_delta_timestamps - TLU_TimeStamp;

    }
    int compareDUT2TLU_normal_Event(eudaq::Event const & ev, const eudaq::Event  & tluEvent)const{

      auto DUT_TimeStamp = ev.GetTimestamp() - m_dut_begin;
      auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;

      return hasTimeOVerlaping(DUT_TimeStamp, DUT_TimeStamp + m_DUT_active_time, TLU_TimeStamp, TLU_TimeStamp + 10);//96000

    }

    int compareDUT2TLU_sync_event(eudaq::Event const & ev, const eudaq::Event  & tluEvent)const{

      auto DUT_TimeStamp = ev.GetTimestamp() - m_dut_begin;
      auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;

      auto eventDiff = TLU_TimeStamp - m_last_tlu;
      auto jitter = eventDiff /m_jitter_denominator + m_jitter_offset;
      auto sync = hasTimeOVerlaping(DUT_TimeStamp + m_default_delta_timestamps-2*jitter, DUT_TimeStamp + m_default_delta_timestamps + 2* jitter, TLU_TimeStamp, TLU_TimeStamp + jitter);
      auto diff = abs(static_cast<__int64>(ev.GetTimestamp() - m_dut_begin + m_default_delta_timestamps) - static_cast<__int64>(TLU_TimeStamp));
      //std::cout << "event time diff: " << eventDiff << " Jitter_Expected: " <<jitter<< " jitter: " << diff << std::endl;

      if (sync == Event_IS_Sync)
      {
        // recalibrate clocks 
#ifdef  _DEBUG
        if (ev.GetTimestamp() + m_default_delta_timestamps < TLU_TimeStamp)
        {
          EUDAQ_THROW("Timestamp offset is negative");
        }

        if (diff > jitter)
        {
          //   EUDAQ_THROW("Offset shift to big");
        }
#endif //  DEBUG
        resync(ev,tluEvent);
      }
      return sync;

    }

  private:

    mutable uint64_t m_dut_begin = 0, m_tlu_begin = 0, m_last_tlu = 0;
    const uint64_t m_jitter_denominator, m_jitter_offset,m_default_delta_timestamps,m_DUT_active_time;
    const bool m_useSyncEvents;

  };

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

  class DataConverterPlugin {
    public:
      typedef Event::t_eventid t_eventid;

      virtual void Initialize(eudaq::Event const &, eudaq::Configuration const &) {}

      virtual unsigned GetTriggerID(eudaq::Event const &) const;
	  virtual int IsSyncWithTLU(eudaq::Event const & ev, const eudaq::Event  & tluEvent) const {
		  // dummy comparator. it is just checking if the event numbers are the same.
		  const TLUEvent *tlu = dynamic_cast<const eudaq::TLUEvent*>(&tluEvent);
		  unsigned triggerID = ev.GetEventNumber();
	  auto tlu_triggerID=tlu->GetEventNumber();
	return compareTLU2DUT(tlu_triggerID,triggerID);
	  }

	  virtual void setCurrentTLUEvent(eudaq::Event & ev,eudaq::TLUEvent const & tlu){
		  ev.SetTag("tlu_trigger_id",tlu.GetEventNumber());
	  }
	  virtual void GetLCIORunHeader(lcio::LCRunHeader &, eudaq::Event const &, eudaq::Configuration const &) const {}
	  

      /** Returns the LCIO version of the event.
       */
      virtual bool GetLCIOSubEvent(lcio::LCEvent & /*result*/, eudaq::Event const & /*source*/) const { return false; }

      /** Returns the StandardEvent version of the event.
       */
      virtual bool GetStandardSubEvent(StandardEvent & /*result*/, eudaq::Event const & /*source*/) const { return false; };

      /** Returns the type of event this plugin can convert to lcio as a pair of Event type id and subtype string.
       */
      virtual t_eventid const & GetEventType() const { return m_eventtype; }


    virtual std::shared_ptr<eudaq::Event> ExtractEventN(std::shared_ptr<eudaq::Event> ev, size_t NumberOfROF) {
      return nullptr;
    }

	  virtual bool isTLU(const Event&){ return false; }

	  virtual unsigned getUniqueIdentifier(const eudaq::Event  & ev){ return m_thisCount; }

    virtual size_t GetNumberOfROF(const eudaq::Event& pac){ return 1; }
      /** The empty destructor. Need to add it to make it virtual.
       */
      virtual ~DataConverterPlugin() {}

    protected:
      /** The string storing the event type this plugin can convert to lcio.
       *  This string has to be set in the constructor of the actual implementations
       *  of the plugin.
       */
      t_eventid m_eventtype;
	  

      /** The protected constructor which automatically registers the plugin
       *  at the pluginManager.
       */
      DataConverterPlugin(std::string subtype);
      DataConverterPlugin(unsigned type, std::string subtype = "");
	  static unsigned m_count;
	  unsigned m_thisCount;
    private:
      /** The private copy constructor and assignment operator. They are not used anywhere, so there is not
       *  even an implementation. Even if the childs default copy constructor is public
       *  the code will not compile if it is called, since it cannot acces this cc, which the
       *  the default cc does.
       */
		DataConverterPlugin(DataConverterPlugin &) = delete;
		DataConverterPlugin & operator = (const DataConverterPlugin &) = delete;
  };





}//namespace eudaq

#endif // EUDAQ_INCLUDED_DataConverterPlugin

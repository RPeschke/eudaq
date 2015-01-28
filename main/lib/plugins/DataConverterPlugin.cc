#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/PluginManager.hh"

#include <iostream>

namespace eudaq {

  unsigned DataConverterPlugin::GetTriggerID(eudaq::Event const &) const {
    return (unsigned)-1;
  }

  DataConverterPlugin::DataConverterPlugin(std::string subtype) :DataConverterPlugin(Event::str2id("_RAW"),subtype)
  {
    //std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;

  }

  DataConverterPlugin::DataConverterPlugin(unsigned type, std::string subtype)
	  : m_eventtype(make_pair(type, subtype)), m_thisCount(m_count)
  {
  //  std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << " unique identifier "<< m_thisCount << " number of instances " <<m_count << std::endl;
    PluginManager::GetInstance().RegisterPlugin(this);
	m_count+=10;
  }
  unsigned DataConverterPlugin::m_count = 0;


  int CompareTimeStampsWithJitter::compareDUT2TLU(eudaq::Event const & ev, const eudaq::Event & tluEvent) const
  {
    if (firstEvent)
    {
      if (!isSetup()){
        EUDAQ_THROW("Parameter for the compare algoritm are not setup correctly");
      }

      m_tlu_begin = tluEvent.GetTimestamp();
      m_dut_begin = ev.GetTimestamp();
      m_last_tlu = 0; //would be tluEvent.GetTimestamp()- m_tlu_begin;
      firstEvent = false;
    }

    if (f_isSync_event)
    {

      if (f_isSync_event(tluEvent))
      {
        return compareDUT2TLU_sync_event(ev, tluEvent);
      }
      else
      {
        return compareDUT2TLU_normal_Event(ev, tluEvent);
      }


    }
    else{

      auto sync = compareDUT2TLU_normal_Event(ev, tluEvent);
      if (sync == Event_IS_Sync)
      {
        resync_jitter(ev, tluEvent);
      }
      return sync;
    }
  }

  void CompareTimeStampsWithJitter::set_Jitter_denominator(uint64_t jitterDenom)
  {
    m_jitter_denominator = jitterDenom;
  }

  void CompareTimeStampsWithJitter::set_Jitter_offset(uint64_t jitter_offset)
  {
    m_jitter_offset = jitter_offset;
  }

  void CompareTimeStampsWithJitter::set_default_delta_timestamp(uint64_t deltaTimestamp)
  {
    m_default_delta_timestamps = deltaTimestamp;
  }

  void CompareTimeStampsWithJitter::set_DUT_active_time(uint64_t DUT_active_time)
  {
    m_DUT_active_time = DUT_active_time;
  }

  bool CompareTimeStampsWithJitter::isSetup() const
  {
    if (m_jitter_denominator == PARAMETER_NOT_SET
        ||
        m_jitter_offset == PARAMETER_NOT_SET
        ||
        m_default_delta_timestamps == PARAMETER_NOT_SET
        ||
        m_DUT_active_time == PARAMETER_NOT_SET
        )
    {
      return false;
    }

    return true;
  }

  void CompareTimeStampsWithJitter::resync_jitter(eudaq::Event const & ev, const eudaq::Event & tluEvent) const
  {
    auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;
    m_last_tlu = TLU_TimeStamp;
#ifdef _DEBUG
    if (ev.GetTimestamp() + m_default_delta_timestamps < TLU_TimeStamp)
    {
      EUDAQ_THROW("Timestamp offset is negative");
    }
#endif
    m_dut_begin = ev.GetTimestamp() + m_default_delta_timestamps - TLU_TimeStamp;
  }

  int CompareTimeStampsWithJitter::compareDUT2TLU_normal_Event(eudaq::Event const & ev, const eudaq::Event & tluEvent) const
  {
    auto DUT_TimeStamp = ev.GetTimestamp() - m_dut_begin;
    auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;

    return hasTimeOVerlaping(DUT_TimeStamp, DUT_TimeStamp + m_DUT_active_time, TLU_TimeStamp, TLU_TimeStamp + 10);//96000
  }

  int CompareTimeStampsWithJitter::compareDUT2TLU_sync_event(eudaq::Event const & ev, const eudaq::Event & tluEvent) const
  {
    auto DUT_TimeStamp = ev.GetTimestamp() - m_dut_begin;
    auto TLU_TimeStamp = tluEvent.GetTimestamp() - m_tlu_begin;

    auto eventDiff = TLU_TimeStamp - m_last_tlu;
    auto jitter = eventDiff / m_jitter_denominator + m_jitter_offset;
    auto sync = hasTimeOVerlaping(DUT_TimeStamp + m_default_delta_timestamps - 2 * jitter, DUT_TimeStamp + m_default_delta_timestamps + 2 * jitter, TLU_TimeStamp, TLU_TimeStamp + jitter);

    if (sync == Event_IS_Sync)
    {
      // recalibrate clocks 
#ifdef  _DEBUG
      auto diff = abs(static_cast<__int64>(ev.GetTimestamp() - m_dut_begin + m_default_delta_timestamps) - static_cast<__int64>(TLU_TimeStamp));
      //std::cout << "event time diff: " << eventDiff << " Jitter_Expected: " <<jitter<< " jitter: " << diff << std::endl;


      if (diff > jitter)
      {
        EUDAQ_THROW("Offset shift to big");
      }
#endif //  DEBUG
      resync_jitter(ev, tluEvent);
    }
    return sync;
  }

  CompareTimeStampsWithJitter::CompareTimeStampsWithJitter(uint64_t jitter_denominator, uint64_t jitter_offset, uint64_t default_delta_timestamps, uint64_t DUT_active_time,isSyncEvent_F isSyncEvent) :
    m_jitter_denominator(jitter_denominator),
    m_jitter_offset(jitter_offset),
    m_default_delta_timestamps(default_delta_timestamps),
    m_DUT_active_time(DUT_active_time),
    f_isSync_event(isSyncEvent)
  {

  }

}//namespace eudaq

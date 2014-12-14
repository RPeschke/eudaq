#include "eudaq/EventSynchronisationMultiTSEvents.hh"

#include <memory>


#define ADDTLUMarker    0x8000000000000000
#define RemoveTLUMarker 0x7FFFFFFFFFFFFFFF

uint64_t AddTLUMarker2TimeStamp(uint64_t timestamp){
  return timestamp | ADDTLUMarker;
  
}

uint64_t RemoveTLUMarkerFromTimeStamp(uint64_t timeStamp){
  return timeStamp & RemoveTLUMarker;
}

bool isTLUmarkedTimeStamp(uint64_t timeStamp){

  return timeStamp > ADDTLUMarker;
}

using namespace std;




namespace eudaq{



  void syncToMultiTSEvents::Process_Event_is_sync(std::shared_ptr<Event> ev, eudaq::Event const & tlu)
  {
    ev->pushTimeStamp(AddTLUMarker2TimeStamp(tlu.GetTimestamp()));


  }

  void syncToMultiTSEvents::Process_Event_is_late(std::shared_ptr<Event> ev, eudaq::Event const & tlu)
  {
    
    for (size_t i = 0; i < ev->GetSizeOfTimeStamps(); ++i){
      if (isTLUmarkedTimeStamp(ev->GetTimestamp(i)))
      {
        
        m_outPutQueue.push(ev);
        
        break;
      }

    }



  }

  syncToMultiTSEvents::syncToMultiTSEvents(bool sync) :SyncBase(sync)
  {

  }

  void syncToMultiTSEvents::makeDetectorEvent()
  {
    if (!m_sync)
    {
      for (auto& e:m_ProducerEventQueue)
      {
        m_outPutQueue.push(e.front());
        e.pop();
      }
    }
  }


  registerSyncClass(syncToMultiTSEvents, "aida");
}
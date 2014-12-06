#include "eudaq/EventSynchronisationBase.hh"

#include "eudaq/Event.hh"

#include <iostream>

#include <memory>
#include "eudaq/PluginManager.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/EventSynchronisationDetectorEvents.hh"
#include "eudaq/EventSynchronisationMultiTSEvents.hh"

#define FILEINDEX_OFFSET 10000
using std::cout;
using std::endl;
using std::shared_ptr;
using namespace std;
namespace eudaq{
  SyncBase::SyncBase(bool sync) :
    m_registertProducer(0),
    m_ProducerEventQueue(0),
    isAsync_(false),
    m_TLUs_found(0),
    m_sync(sync)
  {


  }




  void SyncBase::addBOREDetectorEvent(int fileIndex, const eudaq::DetectorEvent& BOREvent)
  {


    for (size_t i = 0; i < BOREvent.NumEvents(); ++i)
    {
      addBORE_Event(fileIndex, *BOREvent.GetEvent(i));
    }

  }

  void SyncBase::addBORE_Event(int fileIndex, const eudaq::Event& BOREEvent)
  {
  
    ++m_registertProducer;



    if (m_EventsProFileReader.size() < fileIndex + 1)
    {
      m_EventsProFileReader.push_back(0);
    }
    m_EventsProFileReader[fileIndex]++;
    auto identifier = PluginManager::getUniqueIdentifier(BOREEvent);
    if (identifier > FILEINDEX_OFFSET)
    {
      EUDAQ_THROW("the Unique identifier for this Event is to larg. Increase the value of FILEINDEX_OFFSET or return a smaler number from PluginManager::getUniqueIdentifier(BOREEvent)");
    }
    if (PluginManager::isTLU(BOREEvent))
    {
      if (m_TLUs_found == 0)
      {
        m_ProducerId2Eventqueue[getUniqueID(fileIndex, identifier)] = 0;
      }
      else{
        m_ProducerId2Eventqueue[getUniqueID(fileIndex, identifier)] = m_event_id++; //only the first TLU gets threated differently all others are just producers
      }
      ++m_TLUs_found;
    }
    else{

      m_ProducerId2Eventqueue[getUniqueID(fileIndex, identifier)] = m_event_id++;
    }


  }

  bool SyncBase::Event_Queue_Is_Empty()
  {

    for (auto& q : m_ProducerEventQueue)
    {
      if (q.empty())
      {
        return true;
      }
    }
    return false;
  }

  bool SyncBase::SubEventQueueIsEmpty(int FileID)
  {

    for (auto& e : m_ProducerId2Eventqueue)
    {
      if (e.first >= getUniqueID(FileID, 0) && e.first < getUniqueID(FileID + 1, 0))
      {
        if (m_ProducerEventQueue[e.second].empty()){
          return true;
        }
      }

    }


    return false;
  }



  bool SyncBase::getNextEvent(std::shared_ptr<eudaq::Event>&   ev)
  {

    if (!m_outPutQueue.empty())
    {

      ev = m_outPutQueue.front();
      m_outPutQueue.pop();
      return true;
    }
    return false;
  }

  SyncBase::eventqueue_t& SyncBase::getQueuefromId(unsigned producerID)
  {
    if (m_ProducerId2Eventqueue.find(producerID) == m_ProducerId2Eventqueue.end()){

      EUDAQ_THROW("unknown Producer ID " + std::to_string(producerID));
    }

    return m_ProducerEventQueue[m_ProducerId2Eventqueue[producerID]];
  }

  SyncBase::eventqueue_t& SyncBase::getQueuefromId(unsigned fileIndex, unsigned eventIndex)
  {
    return getQueuefromId(getUniqueID(fileIndex, eventIndex));
  }

  SyncBase::eventqueue_t& SyncBase::getFirstTLUQueue()
  {
    return m_ProducerEventQueue[0];
  }


  bool SyncBase::SyncFirstEvent()
  {


    if (!m_sync)
    {
      if (!Event_Queue_Is_Empty())
      {
        makeDetectorEvent();
        return true;
      }
      else
      {
        return false;
      }

    }


    auto& TLU_queue = getFirstTLUQueue();

    while (!Event_Queue_Is_Empty())
    {
      if (compareTLUwithEventQueues(TLU_queue.front()))
      {
        makeDetectorEvent();
        return true;
      }
      else if (!Event_Queue_Is_Empty())
      {
        TLU_queue.pop();
      }
    }
    return false;
  }


  bool SyncBase::compareTLUwithEventQueue(std::shared_ptr<eudaq::Event>& tlu_event, eudaq::SyncBase::eventqueue_t& event_queue)
  {
    int ReturnValue = Event_IS_Sync;

    //std::shared_ptr<TLUEvent> tlu=std::dynamic_pointer_cast<TLUEvent>(tlu_event);



    while (!event_queue.empty())
    {
      auto currentEvent = event_queue.front();

      ReturnValue = PluginManager::IsSyncWithTLU(*currentEvent, *tlu_event);
      if (ReturnValue == Event_IS_Sync)
      {
        Process_Event_is_sync(currentEvent, *tlu_event);
        return true;

      }
      else if (ReturnValue == Event_IS_LATE)
      {

        Process_Event_is_late(currentEvent, *tlu_event);
        isAsync_ = true;
        event_queue.pop();

      }
      else if (ReturnValue == Event_IS_EARLY)
      {
        Process_Event_is_early(currentEvent, *tlu_event);
        isAsync_ = true;
        return false;
      }



    }
    return false;
  }

  bool SyncBase::compareTLUwithEventQueues(std::shared_ptr<eudaq::Event>& tlu_event)
  {
    for (size_t i = 1; i < m_ProducerEventQueue.size(); ++i)
    {
      if (!compareTLUwithEventQueue(tlu_event, m_ProducerEventQueue[i])){
        // could not sync event.
        // TLU event is to early or event queue is empty;
        return false;
      }
    }
    return true;

  }

  bool SyncBase::SyncNEvents(size_t N)
  {

    while (m_outPutQueue.size() <= N)
    {
      if (!SyncFirstEvent())
      {
        return false;
      }

    }
    return true;
  }



  void SyncBase::event_queue_pop()
  {
    for (auto& q : m_ProducerEventQueue)
    {
      q.pop();
    }

  }



  void SyncBase::event_queue_pop_TLU_event()
  {
    getFirstTLUQueue().pop();
  }




  void SyncBase::clearOutputQueue()
  {

    std::queue<std::shared_ptr<eudaq::Event>> empty;
    std::swap(m_outPutQueue, empty);

  }

  void SyncBase::PrepareForEvents()
  {
    m_ProducerEventQueue.resize(m_registertProducer);
    if (m_ProducerEventQueue.empty())
    {

      EUDAQ_THROW("Producer Event queue is empty");
    }
    if (m_ProducerEventQueue.size() == 1)
    {
      m_sync = false; // there is no need to sync anything in this case
    }
    if (!m_sync)
    {
      std::cout << "events not synchronized" << std::endl;
      if (m_TLUs_found == 0)
      {
        for (auto& e : m_ProducerId2Eventqueue)
        {
          e.second--;
        }
      }

    }
    else
    {
      if (m_TLUs_found == 0)
      {
        EUDAQ_THROW("no TLU events found in the data\n for the resynchronisation it is nessasary to have a TLU in the data stream \n for now the synchrounsation only works with the old TLU (date 12.2013)");
      }
      else if (m_TLUs_found > 1)
      {
        std::cout << "more than one TLU detected only the first TLU is used for synchronisation " << std::endl;
      }
    }
  }

  unsigned SyncBase::getUniqueID(unsigned fileIndex, unsigned eventIndex)
  {

    return (fileIndex + 1)*FILEINDEX_OFFSET + eventIndex;
  }

//   unsigned SyncBase::getTLU_UniqueID(unsigned fileIndex)
//   {
//     return getUniqueID(fileIndex, 0);
//   }


  int SyncBase::AddDetectorEventToProducerQueue(int fileIndex, std::shared_ptr<eudaq::DetectorEvent> detEvent)
  {
    if (detEvent){
      for (size_t i = 0; i < detEvent->NumEvents(); ++i){
        AddEventToProducerQueue(fileIndex, detEvent->GetEventPtr(i));
      }
    }
    return true;
  }
  int SyncBase::AddEventToProducerQueue(int fileIndex, std::shared_ptr<eudaq::Event> Ev)
  {
    if (Ev)
    {
      auto identifier = PluginManager::getUniqueIdentifier(*Ev);
      try{
        auto &q = getQueuefromId(fileIndex, identifier);
        q.push(Ev);
      }
      catch (...){
        std::cerr << " error in SyncBase::AddEventToProducerQueue(int fileIndex, std::shared_ptr<eudaq::Event> Ev) \n unkown event id: " << identifier << "\n for the event: \n";
        Ev->Print(cout);
      }

    }
    return true;
  }

  std::unique_ptr<SyncBase> factory_sync_class(const char* name, bool sync)
  {
    if (strcmp(name, "DetectorEvents")==0)
    {
      return std::unique_ptr<SyncBase>(new syncToDetectorEvents(sync));
    }
    else if (strcmp(name, "aida")==0)
    {
      return std::unique_ptr<SyncBase>(new syncToMultiTSEvents(sync));
    }

    return nullptr;
  }

}

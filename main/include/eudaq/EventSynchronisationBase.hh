#ifndef EventSynchronisationBase_h__
#define EventSynchronisationBase_h__


#include "eudaq/DetectorEvent.hh"
#include "eudaq/FileSerializer.hh"
#include <memory>
#include <queue>
// base class for all Synchronization Plugins
// it is desired to be as modular es possible with this approach.
// first step is to separate the events from different Producers. 
// then it will be recombined to a new event
// The Idea is that the user can define a condition that need to be true to define if an event is sync or not

namespace eudaq{




  class DLLEXPORT SyncBase {
  public:
    typedef std::queue<std::shared_ptr<eudaq::Event>> eventqueue_t;


    // public interface

    bool getNextEvent(std::shared_ptr<eudaq::Event>&   ev);
    int AddDetectorEventToProducerQueue(int fileIndex, std::shared_ptr<eudaq::DetectorEvent> dev);
    int AddEventToProducerQueue(int fileIndex, std::shared_ptr<eudaq::Event> Ev);
    void clearOutputQueue();
    void addBOREDetectorEvent(int fileIndex, const eudaq::DetectorEvent& BOREvent);
    void addBORE_Event(int fileIndex, const eudaq::Event& BOREEvent);
    SyncBase(bool sync = true);
    virtual ~SyncBase() {}
    bool SyncNEvents(size_t N);
    void PrepareForEvents();
    bool SubEventQueueIsEmpty(int i);
    bool SyncFirstEvent();
    
  private:
    
    virtual void Process_Event_is_sync(std::shared_ptr<eudaq::Event>   ev, eudaq::Event const & tlu) {}
    virtual void Process_Event_is_late(std::shared_ptr<eudaq::Event>   ev, eudaq::Event const & tlu) {}
    virtual void Process_Event_is_early(std::shared_ptr<eudaq::Event> ev, eudaq::Event const & tlu) {}

    virtual void storeCurrentOrder(){}
    virtual void makeDetectorEvent(){}


    size_t  m_event_id = 0;
    /** The empty destructor. Need to add it to make it virtual.
     */

  protected:

    bool compareTLUwithEventQueues(std::shared_ptr<eudaq::Event>& tlu_event);
    bool compareTLUwithEventQueue(std::shared_ptr<eudaq::Event>& tlu_event, SyncBase::eventqueue_t& event_queue);
    bool Event_Queue_Is_Empty();
    void event_queue_pop_TLU_event();
    void event_queue_pop();
    eventqueue_t& getQueuefromId(unsigned producerID);
    eventqueue_t& getQueuefromId(unsigned fileIndex, unsigned eventIndex);

    eventqueue_t& getFirstTLUQueue();
    unsigned getUniqueID(unsigned fileIndex, unsigned eventIndex);
    //unsigned getTLU_UniqueID(unsigned fileIndex);
    std::map<unsigned, size_t> m_ProducerId2Eventqueue;
    size_t m_registertProducer;
    std::vector<size_t> m_EventsProFileReader;
    /* This vector saves for each producer an event queue */

    std::vector<eventqueue_t> m_ProducerEventQueue;

    std::queue<std::shared_ptr<eudaq::Event>> m_outPutQueue;


    int m_TLUs_found;

    bool isAsync_;
    size_t NumberOfEventsToSync_;
    uint64_t longTimeDiff_;

    bool m_sync;
  };

  std::unique_ptr<SyncBase>  factory_sync_class(const char* name,bool sync);

}//namespace eudaq



#endif // EventSynchronisationBase_h__

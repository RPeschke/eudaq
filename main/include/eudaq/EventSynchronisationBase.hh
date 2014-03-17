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
// for future use it should be possible to check an entire block of data to be sync. 
// the Idea is that one has for example every 1000 events a sync event and one only writes data to disc if all 1000 events are
// correct. 
// The Idea is that the user can define a condition that need to be true to define if an event is sync or not

namespace eudaq{
  



  class DLLEXPORT SyncBase {
    public:
     	 typedef std::queue<std::shared_ptr<eudaq::Event>> eventqueue_t ;

    // virtual int isSynchron();
//	 virtual bool AddNextEventToQueue();
	 int AddDetectorElementToProducerQueue(int fileIndex,std::shared_ptr<eudaq::DetectorEvent> dev );
	 virtual bool SyncFirstEvent();
	 virtual bool SyncNEvents(size_t N);
	 virtual bool getNextEvent( std::shared_ptr<eudaq::DetectorEvent>  & ev);
	// virtual bool SynEvents(FileDeserializer & des, int ver, std::shared_ptr<eudaq::Event>  & ev);
	// virtual int extractCorrect_event(std::shared_ptr<eudaq::Event> tlu_event,eudaq::eventqueue_t& event_queue,std::shared_ptr<eudaq::Event>  &outputEvent);
	 virtual bool compareTLUwithEventQueue(std::shared_ptr<eudaq::Event>& tlu_event,SyncBase::eventqueue_t& event_queue);
	 virtual bool compareTLUwithEventQueues(std::shared_ptr<eudaq::Event>& tlu_event);

	 void storeCurrentOrder();
	 bool Event_Queue_Is_Empty();
	 bool SubEventQueueIsEmpty(int i);
	 void event_queue_pop();
	 void event_queue_pop_TLU_event();
	 void makeDetectorEvent();
	 void clearDetectorQueue();
      /** The empty destructor. Need to add it to make it virtual.
       */
	 virtual ~SyncBase() {}
	 SyncBase();
	 void addBOREEvent(int fileIndex,const eudaq::DetectorEvent& BOREvent);
	 void PrepareForEvents();

    protected:

		eventqueue_t& getQueuefromId(unsigned producerID);
		eventqueue_t& getQueuefromId(unsigned fileIndex,unsigned eventIndex);
	
		eventqueue_t& getFirstTLUQueue();
		unsigned getUniqueID(unsigned fileIndex,unsigned eventIndex);
		unsigned getTLU_UniqueID(unsigned fileIndex);
		std::map<unsigned,size_t> m_ProducerId2Eventqueue;
		size_t m_registertProducer;
		std::vector<size_t> m_EventsProFileReader;
		/* This vector saves for each producer an event queue */
		
		std::vector<eventqueue_t> m_ProducerEventQueue;
		
		std::queue<std::shared_ptr<eudaq::DetectorEvent>> m_DetectorEventQueue;
	
//		int m_ver;
		int m_TLUs_found;
		//int handleEventSync(int syncFlag, eudaq::eventqueue_t& producer_queue);
	//	bool m_queueStatus;
		//long lastAsyncEvent_,currentEvent_;
		bool isAsync_;
		size_t NumberOfEventsToSync_;
		unsigned long long longTimeDiff_;
		
  };

}//namespace eudaq



#endif // EventSynchronisationBase_h__

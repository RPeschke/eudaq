#ifndef genericSynchronisation_h__
#define genericSynchronisation_h__


#define NOUNIQUEIDENTIFIER   0
#define NOFILEINDEX          0
#define NOTTLUFOUND          static_cast<size_t>(-1)


namespace eudaq{



  template <typename T>
  class DLLEXPORT SyncBase {
  public:
    typedef std::shared_ptr<T> epointer;
    typedef std::queue<epointer> eventqueue_t;
    typedef eudaq::genericDetContainer<T> DetectorContainer;
    typedef std::shared_ptr<DetectorContainer> detPointer;
    typedef typename t::t_id ELement_id;
    typedef std::pair<ELement_id, int> ProducerID;


    int AddDetectorElementToProducerQueue(detPointer dev, int fileIndex = NOFILEINDEX){
      for (size_t i = 0; i != dev->Size(); ++i)
      {
        addElemtToProducerQueue(dev->getElement(i), fileIndex);
      }
    }

    // this adds a new element to the Producer queue. 
    // to have the possibility to have two producer of the same time the caller can set a unique identifier 
    // to separate two events of the same type and subtype
    int addElemtToProducerQueue(epointer ev, int UniqueIdentifier = NOUNIQUEIDENTIFIER){
      getQueuefromId(ev, UniqueIdentifier).push(ev);
    }

    virtual bool SyncFirstEvent();
    virtual bool SyncNEvents(size_t N);
    virtual bool getNextEvent(detPointer  & ev);

    virtual bool compareTLUwithEventQueue(epointer tlu_event, eventqueue_t& event_queue);
    virtual bool compareTLUwithEventQueues(epointer tlu_event);

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
 
    void addBOREEvent(detPointer BORContainer, int fileIndex = NOFILEINDEX){
      for (size_t i = 0; i != BORContainer->Size();++i)
      {
        addBOREEvent(BORContainer->getElement(i), fileIndex);
      }

    }
    void addBOREEvent(epointer BOREvent, int UniqueIdentifier = NOUNIQUEIDENTIFIER){
     
      static size_t id = 0;

      bool isTLU = isTLUContainer(BOREvent);
      
      try{
        getQueuefromId(BOREvent, UniqueIdentifier);// this should fail because it should be a new producer which is not registered yet
        EUDAQ_THROW("Producer already registered");

      }
      catch (...){
        if (isTLU){
          ++m_TLUs_found;
          m_TLU_NumberToEventQueue[m_TLUs_found] = id;
        }
        m_ProducerId2Eventqueue[getUniqueID(BOREvent, UniqueIdentifier)] = id++;
        m_ProducerEventQueue.push_back(eventqueue_t());
        m_ProducerEventQueue.back().push(BOREvent);
        m_registertProducer = m_ProducerEventQueue.size();
      }
    }
    void PrepareForEvents();

  protected:  

    eventqueue_t& getQueuefromId(ProducerID producerID){
      if (m_ProducerId2Eventqueue.find(producerID) == m_ProducerId2Eventqueue.end()){

        EUDAQ_THROW("unknown Producer ID ");
      }

      return m_ProducerEventQueue[m_ProducerId2Eventqueue[producerID]];

    }
    eventqueue_t& getQueuefromId(epointer ev, int UniqueIdentifier){

      return getQueuefromId(getUniqueID(ev, UniqueIdentifier));
    }

    eventqueue_t& getFirstTLUQueue(){
      return m_ProducerEventQueue[m_TLU_NumberToEventQueue[1]];
    }
  

    ProducerID getUniqueID(epointer ev, int UniqueIdentifier){
      return ProducerID(ev->getID(), UniqueIdentifier);
    }

    std::map<ProducerID, size_t>    m_ProducerId2Eventqueue;
    std::map<unsigned, size_t>      m_TLU_NumberToEventQueue;
    std::vector<size_t>             m_EventsProFileReader;
    
    /* This vector saves for each producer an event queue */
    std::vector<eventqueue_t>       m_ProducerEventQueue;

    std::queue<detPointer>          m_DetectorEventQueue;


    size_t m_registertProducer = 0;
    int m_TLUs_found=0;
    size_t m_first_TLU_event_queue=NOTTLUFOUND;
    bool isAsync_=0;
    size_t NumberOfEventsToSync_=1;


  };

}//namespace eudaq



#endif // genericSynchronisation_h__

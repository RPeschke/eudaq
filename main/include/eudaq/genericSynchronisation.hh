#ifndef genericSynchronisation_h__
#define genericSynchronisation_h__

#include "eudaq/resyncUtilities.hh"
#include <deque>

#define NOUNIQUEIDENTIFIER   0
#define NOFILEINDEX          0
#define NOTLUFOUND          static_cast<size_t>(-1)


namespace eudaq{



  template <typename T>
  class DLLEXPORT GenericPacketSync {
  public:
    
    typedef std::shared_ptr<T> epointer;
    typedef std::pair<epointer, size_t> elementOfIntresst;
    typedef std::deque<epointer> eventqueue_t;
    typedef eudaq::genericDetContainer<T> DetectorContainer;
    typedef std::shared_ptr<DetectorContainer> detPointer;
    typedef typename T::t_id ELement_id;
    typedef std::pair<ELement_id, int> ProducerID;

    static elementOfIntresst createElementOfIntresst(epointer ev,size_t id=0){

      return elementOfIntresst(ev,id);
    }
    static bool nextElement(eventqueue_t& equeue){
    
      if (PluginManager<T>::NextNumerator(*equeue.front()))
      {
        return true;
      }
      equeue.pop_front();
      if (!equeue.empty())
      {

        PluginManager<T>::getElement(*equeue.front(), 0);
        return true;
      }
      return false;
    }
    static bool isReferenceToSameQueue(eventqueue_t& a, eventqueue_t& b){
      return &a == &b;
    }

    void AddDetectorElementToProducerQueue(detPointer dev, int fileIndex = NOFILEINDEX){
      for (size_t i = 0; i != dev->Size(); ++i)
      {
        addElemtToProducerQueue(dev->getElement(i), fileIndex);
      }
    }

    // this adds a new element to the Producer queue. 
    // to have the possibility to have two producer of the same time the caller can set a unique identifier 
    // to separate two events of the same type and subtype
    void addElemtToProducerQueue(epointer ev, int UniqueIdentifier = NOUNIQUEIDENTIFIER){
      getQueuefromId(ev, UniqueIdentifier).push_back(ev);
    }

    bool SyncFirstEvent(){
      if (m_ProducerEventQueue.size() == 0)
      {
        EUDAQ_THROW("Producer Event queue is empty");
      }
      auto& TLU_queue = getMainTLUQueue();
      while (!Event_Queue_Is_Empty())
      {
        if (compareTLUwithEventQueues(TLU_queue.front()))
        {
          makeDetectorEvent();
          return true;
        }
        else if (!Event_Queue_Is_Empty())
        {
          nextElement(TLU_queue);
          //TLU_queue.pop_front();
        }
      }
      return false;
    }
    bool SyncNEvents(size_t N);

    bool getNextEvent(detPointer  & ev){

      if (!m_output.empty())
      {

        ev = m_output.front();
        m_output.pop();
        return true;
      }
      return false;
    }
    
    int compareTLUwithDUT(epointer tlu_event, epointer dut_event){
      for (size_t i=)
      {
      }

    }

    bool compareTLUwithEventQueue(epointer tlu_event, eventqueue_t& event_queue){
      int ReturnValue = Event_IS_Sync;
      std::cout << "TLU  " << AidaPacket::type2str(tlu_event->GetPacketType()) << "  time stamp " << tlu_event->GetEventNumber() << std::endl;
     
      while (!event_queue.empty())
      {
        auto& currentEvent = event_queue.front();
      
        ReturnValue = PluginManager<T>::IsSyncWithTLU(*currentEvent, *tlu_event);
        if (ReturnValue == Event_IS_Sync)
        {
          std::cout << "Event_IS_Sync" << std::endl;
          return true;
        }
        else if (ReturnValue == Event_IS_LATE)
        {
          std::cout << "Event_IS_LATE" << std::endl;
          isAsync_ = true;
          nextElement(event_queue);
          //event_queue.pop_front();
        }
        else if (ReturnValue == Event_IS_EARLY)
        {
          std::cout << "Event_IS_EARLY" << std::endl;
          isAsync_ = true;
          return false;
        }
      }
      return false;
    }


    bool compareTLUwithEventQueues(epointer tlu_event)
    {
      for (auto& e : m_ProducerEventQueue)
      {
        if (isReferenceToSameQueue(e,getMainTLUQueue()))
        {
          continue;
        }
        if (!compareTLUwithEventQueue(tlu_event, e)){
          // could not sync event.
          // TLU event is to early or event queue is empty;
          return false;
        }
      }
      return true;
    }

    void storeCurrentOrder();
    bool Event_Queue_Is_Empty(){
      for (auto& q : m_ProducerEventQueue)
      {
        if (q.empty())
        {
          return true;
        }
      }
      return false;
    }

    bool SubEventQueueIsEmpty(int i);
    void event_queue_pop(){
      for (auto& q : m_ProducerEventQueue)
      {
        nextElement(q);
        
      }
    }
    void event_queue_pop_TLU_event(){
      nextElement(getMainTLUQueue());
    }
    void makeDetectorEvent(){
      auto &TLU = getMainTLUQueue().front();
      auto det = make_shared<DetectorContainer>(*TLU);
     // det->add_element(TLU);
      for (auto& e : m_ProducerEventQueue)
      {

        det->add_element(e.front());

      }


      m_output.push(det);
      event_queue_pop_TLU_event();
      //event_queue_pop();

    }
    void clearDetectorQueue(){

      while (!m_output.empty())
      {
        m_output.pop();
      }
    }
    /** The empty destructor. Need to add it to make it virtual.
     */
    virtual ~GenericPacketSync() {}

    void addBOREEvent(detPointer BORContainer, int fileIndex = NOFILEINDEX){
      for (size_t i = 0; i != BORContainer->Size(); ++i)
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
          
          m_TLU_NumberToEventQueue[m_TLUs_found] = id;
          ++m_TLUs_found;
        }
        m_ProducerId2Eventqueue[getUniqueID(BOREvent, UniqueIdentifier)] = id++;
        m_ProducerEventQueue.push_back(eventqueue_t());
        m_ProducerEventQueue.back().push_back(BOREvent);
        m_registertProducer = m_ProducerEventQueue.size();
      }
    }
    void PrepareForEvents(){
      if (m_TLUs_found == 0)
      {
        EUDAQ_THROW("no TLU events found in the data\n for the resynchronisation it is nessasary to have a TLU in the data stream \n for now the synchrounsation only works with the old TLU (date 12.2013)");
      }
      else if (m_TLUs_found > 1)
      {
        //EUDAQ_THROW("to many TLUs in the data stream.\n the sync mechanism only works with 1 TLU");
        std::cout << "more than one TLU detected only the first TLU is used for synchronization " << std::endl;
      }
      event_queue_pop();
    }
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

    eventqueue_t& getMainTLUQueue(){
      return m_ProducerEventQueue[m_TLU_NumberToEventQueue[m_main_TLU]];
    }


    ProducerID getUniqueID(epointer ev, int UniqueIdentifier){
      return ProducerID(ev->getID(), UniqueIdentifier);
    }

    std::map<ProducerID, size_t>    m_ProducerId2Eventqueue;
    std::map<unsigned, size_t>      m_TLU_NumberToEventQueue;
    std::vector<size_t>             m_EventsProFileReader;

    /* This vector saves for each producer an event queue */
    std::vector<eventqueue_t>       m_ProducerEventQueue;

    std::queue<detPointer>          m_output;


    size_t m_registertProducer = 0;
    int m_TLUs_found = 0;
    size_t m_main_TLU = 0;
    size_t m_first_TLU_event_queue = NOTLUFOUND;
    bool isAsync_ = 0;
    size_t NumberOfEventsToSync_ = 1;


  };

}//namespace eudaq



#endif // genericSynchronisation_h__

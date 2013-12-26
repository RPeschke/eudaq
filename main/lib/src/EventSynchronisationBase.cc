#include "eudaq\EventSynchronisationBase.hh"
//#include "eudaq\counted_ptr.hh"
#include "eudaq/Event.hh"

#include <iostream>

#include <memory>
#include "eudaq/PluginManager.hh"
using std::cout;
using std::endl;
using std::shared_ptr;
using namespace std;
namespace eudaq{
bool SyncBase::SynEvents( FileDeserializer & des, int ver, std::shared_ptr<eudaq::Event>  & ev )
{
	if (m_ver==0)
	{
		m_des=&des;
		m_ver=ver;
	}
		
	return getNextEvent(ev);
}

bool SyncBase::getNextEvent(  std::shared_ptr<eudaq::Event>  & ev )
{
	//SyncFirstEvent();
	if (m_DetectorEventQueue.size()<100)
	{
		if(!SyncNEvents(NumberOfEventsToSync_)){
			return false;
		}
	}
	
		if (!m_DetectorEventQueue.empty())
		{
			
			ev=m_DetectorEventQueue.front();
			m_DetectorEventQueue.pop();
			return true;
		}
	return false;
}

SyncBase::SyncBase( size_t numberOfProducer ,size_t NumberOfEventsToSync,unsigned long long longTimeDiff):
	m_registertProducer(0),
	m_ProducerEventQueue(numberOfProducer),
	m_des(nullptr),m_ver(0),lastAsyncEvent_(0),currentEvent_(0),NumberOfEventsToSync_(NumberOfEventsToSync),longTimeDiff_(longTimeDiff)
{
	//cout<<numberOfProducer<<endl;
}




eudaq::eventqueue_t& SyncBase::getQueuefromId( unsigned producerID )
{
	if(m_ProducerId2Eventqueue.find(producerID)==m_ProducerId2Eventqueue.end()){
 		m_ProducerId2Eventqueue[producerID]=m_registertProducer++;
 	}
//	cout<<m_ProducerId2Eventqueue[producerID]<<endl;
	return m_ProducerEventQueue[m_ProducerId2Eventqueue[producerID]];
}

bool SyncBase::AddNextEventToQueue()
{
	if (event_queue_size()<5)
	{
	
		std::shared_ptr<eudaq::Event> ev;
		m_queueStatus=m_des->ReadEvent(m_ver,ev);
		if (m_queueStatus)
			{
		
	
				std::shared_ptr<eudaq::DetectorEvent> detEvent=std::dynamic_pointer_cast<eudaq::DetectorEvent>(ev);
				for(size_t i=0;i< detEvent->NumEvents();++i){
				//	cout<<detEvent->GetEventPtr(i)->get_id()<<endl;
					auto &q=getQueuefromId(i);
					q.m_queue.push_back(detEvent->GetEventPtr(i));
				}
			}
		return m_queueStatus;
	}
	return true;
}



bool SyncBase::SyncFirstEvent()
{

	if (m_ProducerEventQueue.size()==0)
	{
		
		EUDAQ_THROW("Producer Event queue is empty");
	}
	auto& TLU_queue=m_ProducerEventQueue.begin();
	//	auto beginTLU=PluginManager::GetTimeStamp(*(TLU->m_queue.front()));
	//	auto endTLU=beginTLU+PluginManager::GetTimeDuration(*(TLU->m_queue.front()));
	//(unsigned runnumber, unsigned eventnumber, unsigned long long timestamp) :


	if (Event_Queue_Is_Empty())
	{
		if (!AddNextEventToQueue())
		{
			return false;
		}
		
	}

	do 
	{
		++currentEvent_;
		if (compareTLUwithEventQueues(TLU_queue->front()))
		{
			makeDetectorEvent();
			return true;
		}else
		{
			TLU_queue->pop();
		}
		
		if (Event_Queue_Is_Empty())
		{
			if (!AddNextEventToQueue())
			{
				//return false;
				cout<<"no more Elements"<<endl;
			}

		}
	} while (!Event_Queue_Is_Empty());

	
	


	return false;
}

bool SyncBase::SyncNEvents( int N )
{
	while (m_DetectorEventQueue.size()<=2*N)
	{
		if (!SyncFirstEvent())
		{
			return false;
		}
		if (lastAsyncEvent_==currentEvent_-1)
		{
			auto last_element=m_DetectorEventQueue.back();
			clearDetectorQueue();
			m_DetectorEventQueue.push(last_element);
		}
	}
	return true;
}

void SyncBase::makeDetectorEvent()
{
	auto &TLU=m_ProducerEventQueue[0].front();
	shared_ptr<DetectorEvent> det=make_shared<DetectorEvent>(TLU->GetRunNumber(),TLU->GetEventNumber(),TLU->GetTimestamp());
	det->AddEvent(TLU);
	for (size_t i=1;i<m_ProducerEventQueue.size();++i)
	{

		det->AddEvent(m_ProducerEventQueue[i].front());

	}

	//	TLU_queue->m_queue.pop();
	m_DetectorEventQueue.push(det);
	event_queue_pop();
	
}

void SyncBase::event_queue_pop()
{
	for (auto &q:m_ProducerEventQueue)
	{
		
			q.pop();
		
	}

}

size_t SyncBase::event_queue_size()
{size_t returnValue=(size_t)-1;
	for (auto &q:m_ProducerEventQueue)
	{
		if (returnValue>q.size())
		{
			returnValue=q.size();
		}
	}
	return returnValue;
}

bool SyncBase::compareTLUwithEventQueue( std::shared_ptr<eudaq::Event>& tlu_event,eudaq::eventqueue_t& event_queue )
{
	int ReturnValue=0;

	std::shared_ptr<TLUEvent> tlu=std::dynamic_pointer_cast<TLUEvent>(tlu_event);

	auto& currentEvent=event_queue.begin();

	do
	{

		//shared_ptr<eudaq::Event> currentEvent=event_queue.m_queue.front();


	//	cout<<"************************************************************************"<<endl;
	//	cout<<Event::id2str(PluginManager::getEventId(**currentEvent).first)<< "  "<<PluginManager::getEventId(**currentEvent).second<<endl;
		ReturnValue=PluginManager::IsSyncWithTLU(**currentEvent,*tlu);
		if (ReturnValue== Event_IS_Sync )
		{
			
			return true;
			
		}
		else if (ReturnValue==Event_IS_LATE)
		{
			lastAsyncEvent_=currentEvent_;
			event_queue.pop();
			if(!AddNextEventToQueue()) {
				return false;
			}
		}else if (ReturnValue==Event_IS_EARLY)
		{
			lastAsyncEvent_=currentEvent_;
			return false;
		}

		currentEvent=event_queue.begin();


	}while (currentEvent!=event_queue.end());
	//	cout<<"PluginManager::GetTimeDuration():  "<<PluginManager::GetTimeDuration(*(i->m_queue.front()))<<endl;
	//	cout<<"PluginManager::GetTimeStamp():  "<<PluginManager::GetTimeStamp(*(i->m_queue.front()))<<endl;
	//cout<<"************************************************************************"<<endl;
	//cout<<"PluginManager::GetEventType():  "<<PluginManager::GetEventType(*(i->m_queue.front()))<<endl;
	return false;
}

bool SyncBase::compareTLUwithEventQueues( std::shared_ptr<eudaq::Event>& tlu_event )
{
	for (size_t i=1;i<m_ProducerEventQueue.size();++i)
	{
		if(!compareTLUwithEventQueue( tlu_event,m_ProducerEventQueue[i])){
			// could not sync event.
			// TLU event is to early or event queue is empty;
			return false;
		}
	}
	return true;
}

bool SyncBase::Event_Queue_Is_Empty()
{
	
	for (auto &q:m_ProducerEventQueue)
	{
		if (q.empty())
		{
			return true;
		}
	}
	return false;
}

void SyncBase::clearDetectorQueue()
{
	
		std::queue<std::shared_ptr<eudaq::DetectorEvent>> empty;
		std::swap( m_DetectorEventQueue, empty );
	
}

}
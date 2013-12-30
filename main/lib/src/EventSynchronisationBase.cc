#include "eudaq\EventSynchronisationBase.hh"
//#include "eudaq\counted_ptr.hh"
#include "eudaq/Event.hh"

#include <iostream>

#include <memory>
#include "eudaq/PluginManager.hh"
#include "eudaq/Configuration.hh"
using std::cout;
using std::endl;
using std::shared_ptr;
using namespace std;
namespace eudaq{
SyncBase::SyncBase(const eudaq::DetectorEvent& BOREvent):
	m_registertProducer(0),
	m_ProducerEventQueue(BOREvent.NumEvents()),
	m_des(nullptr),m_ver(0),NumberOfEventsToSync_(1),longTimeDiff_(0),isAsync_(false)
{
	eudaq::Configuration conf(BOREvent.GetTag("CONFIG"));
	conf.SetSection("EventStruct");

	
	

	longTimeDiff_=conf.Get("LongBusyTime",longTimeDiff_); //from config file
	longTimeDiff_=BOREvent.GetTag("longTimeDelay",longTimeDiff_);//from command line
	
	NumberOfEventsToSync_=conf.Get("NumberOfEvents",NumberOfEventsToSync_); //from config file
	NumberOfEventsToSync_=BOREvent.GetTag("NumberOfEvents",NumberOfEventsToSync_);//from command line
	registerEvent(BOREvent);

	//cout<<numberOfProducer<<endl;
}




void SyncBase::registerEvent(const eudaq::DetectorEvent &ev )
{
	const unsigned int TLU_ID=Event::str2id("_TLU");
	size_t id=1;
	int TLUs_found=0;
	for (unsigned i=0;i<ev.NumEvents();++i)
	{
		if (TLU_ID==ev.GetEvent(i)->get_id())
		{
			 m_ProducerId2Eventqueue[i]=0;
			 ++TLUs_found;
		}else{

			m_ProducerId2Eventqueue[i]=id++;
		}
		
	}
	if (TLUs_found==0)
	{
		EUDAQ_THROW("no TLU events found in the data\n for the resynchronisation it is nessasary to have a TLU in the data stream \n for now the synchrounsation only works with the old TLU (date 12.2013)");
	}else if (TLUs_found>1)
	{
		EUDAQ_THROW("to many TLUs in the data stream.\n the sync mechanism only works with 1 TLU");
	}
}

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
	if (m_DetectorEventQueue.size()<NumberOfEventsToSync_)
	{
		if(!SyncNEvents(NumberOfEventsToSync_*2)){
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

SyncBase::eventqueue_t& SyncBase::getQueuefromId( unsigned producerID )
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
					q.push(detEvent->GetEventPtr(i));
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
		//++currentEvent_;
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

bool SyncBase::SyncNEvents( size_t N )
{
	while (m_DetectorEventQueue.size()<=N)
	{
		if (!SyncFirstEvent())
		{
			return false;
		}
		if (isAsync_)
		{
			isAsync_=false;
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

bool SyncBase::compareTLUwithEventQueue( std::shared_ptr<eudaq::Event>& tlu_event,eudaq::SyncBase::eventqueue_t& event_queue )
{
	int ReturnValue=0;

	std::shared_ptr<TLUEvent> tlu=std::dynamic_pointer_cast<TLUEvent>(tlu_event);

	
	//std::queue<std::shared_ptr<eudaq::Event>>::reference currentEvent;
	while(!event_queue.empty())
	{
		auto& currentEvent=event_queue.front();
		//shared_ptr<eudaq::Event> currentEvent=event_queue.m_queue.front();


	//	cout<<"************************************************************************"<<endl;
	//	cout<<Event::id2str(PluginManager::getEventId(**currentEvent).first)<< "  "<<PluginManager::getEventId(**currentEvent).second<<endl;
		ReturnValue=PluginManager::IsSyncWithTLU(*currentEvent,*tlu);
		if (ReturnValue== Event_IS_Sync )
		{
			
			return true;
			
		}
		else if (ReturnValue==Event_IS_LATE)
		{
			//lastAsyncEvent_=currentEvent_;
			isAsync_=true;
			event_queue.pop();
			if(!AddNextEventToQueue()) {
				return false;
			}
		}else if (ReturnValue==Event_IS_EARLY)
		{
			//lastAsyncEvent_=currentEvent_;
			isAsync_=true;
			return false;
		}



	}
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
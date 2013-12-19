#include "eudaq\EventSynchronisationBase.hh"
//#include "eudaq\counted_ptr.hh"
#include "eudaq/Event.hh"

#include <iostream>

#include <memory>
#include "eudaq/PluginManager.hh"
using std::cout;
using std::endl;
using std::shared_ptr;
namespace eudaq{
bool SyncBase::SynEvents( FileDeserializer & des, int ver, eudaq::Event * & ev )
{
	
	do{
	
		des.ReadEvent(ver,ev);
	eudaq::DetectorEvent* detEvent=dynamic_cast<eudaq::DetectorEvent*>(ev);
	for(size_t i=0;i< detEvent->NumEvents();++i){
		cout<<detEvent->GetEventPtr(i)->get_id()<<endl;
		auto &q=getQueuefromId(i);
		q.m_queue.push(detEvent->GetEventPtr(i));
	}
	}while(!isSynchron());
	
	
	
	

	
	
	return true;
}

SyncBase::SyncBase( size_t numberOfProducer ):m_registertProducer(0),m_ProducerEventQueue(numberOfProducer)
{
	cout<<numberOfProducer<<endl;
}

int SyncBase::isSynchron()
{
	if (m_ProducerEventQueue.size()==0)
	{
		return 0;
	}
	auto TLU=m_ProducerEventQueue.begin();
	auto beginTLU=PluginManager::GetTimeStamp(*(TLU->m_queue.front()));
	auto endTLU=beginTLU+PluginManager::GetTimeDuration(*(TLU->m_queue.front()));
	std::shared_ptr<TLUEvent> tlu=std::dynamic_pointer_cast<TLUEvent>(TLU->m_queue.front());
	for (auto i=(m_ProducerEventQueue.begin());i!=m_ProducerEventQueue.end();++i)
	{
		cout<<"************************************************************************"<<endl;
		cout<<Event::id2str(PluginManager::getEventId(*(i->m_queue.front())).first)<< "  "<<PluginManager::getEventId(*(i->m_queue.front())).second<<endl;
		cout<<PluginManager::IsSyncWithTLU(*(i->m_queue.front()),*tlu)<<endl;
		cout<<"i->m_queue.front()->GetTimestamp():  "<<i->m_queue.front()->GetTimestamp()<<endl;
	//	cout<<"PluginManager::GetTimeDuration():  "<<PluginManager::GetTimeDuration(*(i->m_queue.front()))<<endl;
	//	cout<<"PluginManager::GetTimeStamp():  "<<PluginManager::GetTimeStamp(*(i->m_queue.front()))<<endl;
		cout<<"************************************************************************"<<endl;
		//cout<<"PluginManager::GetEventType():  "<<PluginManager::GetEventType(*(i->m_queue.front()))<<endl;
	}
	return 0;
}

void SyncBase::AddNextEventToQueue()
{

}

eudaq::eventqueue_t& SyncBase::getQueuefromId( unsigned producerID )
{
	if(m_ProducerId2Eventqueue.find(producerID)==m_ProducerId2Eventqueue.end()){
 		m_ProducerId2Eventqueue[producerID]=m_registertProducer++;
 	}
	cout<<m_ProducerId2Eventqueue[producerID]<<endl;
	return m_ProducerEventQueue[m_ProducerId2Eventqueue[producerID]];
}

}
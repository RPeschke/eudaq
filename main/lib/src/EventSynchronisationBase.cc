#include "eudaq\EventSynchronisationBase.hh"
#include "eudaq\counted_ptr.hh"
#include "eudaq/Event.hh"

#include <iostream>

#include <memory>
using std::cout;
using std::endl;
using std::shared_ptr;
namespace eudaq{
bool SyncBase::SynEvents( FileDeserializer & des, int ver, eudaq::Event * & ev )
{
	eudaq::Event* eve=nullptr;
	while(!isSynchron()){
	des.ReadEvent(ver,eve);

	eudaq::DetectorEvent* detEvent=dynamic_cast<eudaq::DetectorEvent*>(eve);

	cout<<eve->get_id()<<endl;
	cout<<eudaq::Event::id2str(eve->get_id())<<endl;
	auto &q=getQueuefromId(eve->get_id());
	q.m_queue.push(shared_ptr<eudaq::Event>(eve));

	}
	eve->GetEventNumber();
	return true;
}

SyncBase::SyncBase( size_t numberOfProducer ):m_registertProducer(0),m_ProducerEventQueue(numberOfProducer)
{
	cout<<numberOfProducer<<endl;
}

int SyncBase::isSynchron()
{

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
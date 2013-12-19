#include "eudaq\EventQueue.hh"
#include "eudaq\PluginManager.hh"

/*

unsigned eudaq::eventqueue_t::producers() const
{
	return offsets.size();
}

bool eudaq::eventqueue_t::isempty() const
{
	for (size_t i = 0; i < offsets.size(); ++i) {
		if (events(i) == 0) {
			return true;
		}
	}
	return false;
}

size_t eudaq::eventqueue_t::events( size_t producer ) const
{
	std::list<item_t>::const_iterator it = iter(producer, -1);
	if (it == items.begin()) return 0;
	return std::distance(items.begin(), it);
}

size_t eudaq::eventqueue_t::fullevents() const
{
	size_t min = events(0);
	for (size_t i = 1; i < offsets.size(); ++i) {
		size_t evts = events(i);
		if (evts < min) min = evts;
	}
	return min;
}

void eudaq::eventqueue_t::push( eudaq::Event * ev )
{
	DetectorEvent * dev = dynamic_cast<DetectorEvent *>(ev);
	items.push_front(item_t(dev));
}

void eudaq::eventqueue_t::discardevent( size_t producer )
{
	--offsets[producer];
}

int eudaq::eventqueue_t::clean_back()
{
	int result = 0;
	bool done = false;
	while (!done) {
		for (size_t i = 0; i < producers(); ++i) {
			std::list<item_t>::const_iterator it = offsets.at(i);
			if (it == items.end()) return result;
			++it;
			if (it == items.end()) {
				done = true;
				offsets[i] = items.end();
			}
		}
		++result;
		items.pop_back();
	}
	return result;
}
*/
// eudaq::DetectorEvent * eudaq::eventqueue_t::popevent()
// {
// 	unsigned run = getevent(0).GetRunNumber();
// 	unsigned evt = getevent(0).GetEventNumber();
// 	unsigned long long ts = NOTIMESTAMP;
// 	for (size_t i = 0; i < producers(); ++i) {
// 		if (getevent(i).get_id() == Event::str2id("_TLU")) {
// 			run = getevent(i).GetRunNumber();
// 			evt = getevent(i).GetEventNumber();
// 			ts = getevent(i).GetTimestamp();
// 			break;
// 		}
// 	}
// 	DetectorEvent * dev = new DetectorEvent(run, evt, ts);
// 	for (size_t i = 0; i < producers(); ++i) {
// 		dev->AddEvent(iter(i)->event->GetEventPtr(i));
// 		--offsets[i];
// 	}
// 	bool more = true;
// 	do {
// 		for (size_t i = 0; i < producers(); ++i) {
// 			/*std::list<item_t>::const_iterator it =*/ iter(i, -1);
// 		}
// 		clean_back();
// 		for (size_t i = 0; i < producers(); ++i) {
// 			/*std::list<item_t>::const_iterator it =*/ iter(i, -1); //, true);
// 		}
// 		more = true;
// 		for (size_t i = 0; i < producers(); ++i) {
// 			if (iter(i, -1) == items.end()) {
// 				more = false;
// 				break;
// 			}
// 		}
// 	} while (more);
// 	return dev;
// }
// 
// void eudaq::eventqueue_t::debug( std::ostream & os ) const
// {
// 	os << "empty=" << (isempty()?"yes":"no") << std::flush;
// 	os << " fullevents=" << fullevents() << std::flush
// 		<< " events=" << events(0) << std::flush;
// 	for (size_t i = 1; i < producers(); ++i) {
// 		os << "," << events(i) << std::flush;
// 	}
// }
// 
// std::list<eudaq::eventqueue_t::item_t>::const_iterator eudaq::eventqueue_t::iter( size_t producer, int offset /*= 0*/ ) const
// {
// 	std::list<item_t>::const_iterator it = offsets.at(producer);
// 	for (int i = 0; i <= offset; ++i) {
// 		if (it == items.begin()) EUDAQ_THROW("Bad offset in ResyncTLU routine");
// 		--it;
// 	}
// 	return it;
// }
// 
// unsigned eudaq::eventqueue_t::getid( size_t producer, size_t offset /*= 0*/ ) const
// {
// 	unsigned diff = 0;
// 	if (firstid != (unsigned)-1 && getevent(producer).get_id() == Event::str2id("_TLU")) {
// 		diff = firstid;
// 	}
// 	return (iter(producer, offset)->triggerids[producer] + diff) & IDMASK;
// }
// 
// const eudaq::Event & eudaq::eventqueue_t::getevent( size_t producer, int offset /*= 0*/ ) const
// {
// 	return *iter(producer, offset)->event->GetEvent(producer);
// }
// 
// eudaq::eventqueue_t::item_t::item_t( DetectorEvent * ev /*= 0*/ ) : event(ev)
// {
// 	if (ev) {
// 		for (size_t i = 0; i < ev->NumEvents(); ++i) {
// 			triggerids.push_back(PluginManager::GetTriggerID(*ev->GetEvent(i)));
// 		}
// 	}
// }

void eudaq::eventqueue_t::debug( std::ostream & os ) const
{

}

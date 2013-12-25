#ifndef EventQueue_h__
#define EventQueue_h__


//#include <vector>
#include "eudaq\DetectorEvent.hh"
#include <queue>
#include <memory>

namespace eudaq {
class DLLEXPORT eventqueue_t {
public:
void debug(std::ostream & os) const;

std::vector<std::shared_ptr<eudaq::Event>> m_queue;

void pop();
size_t size();
std::vector<std::shared_ptr<eudaq::Event>>::iterator begin();
std::vector<std::shared_ptr<eudaq::Event>>::iterator end();
std::vector<std::shared_ptr<eudaq::Event>>::reference front();

bool empty();
};

}
#endif // EventQueue_h__

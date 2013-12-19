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

std::queue<std::shared_ptr<eudaq::Event>> m_queue;


};

}
#endif // EventQueue_h__

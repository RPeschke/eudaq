#ifndef PacketEvent_h__
#define PacketEvent_h__
#include "Event.hh"
#include <memory>

namespace eudaq{

  class AidaPacket;
  typedef size_t element_id;
  typedef std::shared_ptr<AidaPacket> epointer;


  class PacketEvent : public Event{

  public: 
    PacketEvent(epointer Packet, element_id id){
      
    }
    epointer m_packet;
    element_id m_element_ID=0;

  };


}
#endif // PacketEvent_h__

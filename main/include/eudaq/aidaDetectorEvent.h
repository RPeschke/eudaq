#ifndef aidaDetectorEvent_h__
#define aidaDetectorEvent_h__

#include "AidaPacket.hh"

namespace eudaq {
class aidaDetectorEvent: public AidaPacket{

	virtual void Serialize(Serializer &) const;
};

}
#endif // aidaDetectorEvent_h__

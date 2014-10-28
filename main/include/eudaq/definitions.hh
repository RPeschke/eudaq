#ifndef definitions_h__
#define definitions_h__

#include <stdint.h>

namespace std{
	class string;
}

namespace eudaq{
	
	class Event;
	using event_mainType = unsigned;
	using event_subtype = std::string;


	class AidaPacket;
	using packetData_t = uint64_t;
	using packet_mainType = packetData_t;
	using packet_subType = packetData_t;

}

#endif // definitions_h__

#ifndef eventPacketAIDAdataConverterPlugin_h__
#define eventPacketAIDAdataConverterPlugin_h__
#include "eudaq/aidaDataConverterPlugin.hh"

namespace eudaq{
  static const char* eventType="_raw";
  static const char* subEventType = "even";


  class eventPacketAIDAdataConverterPlugin: public aidaDataConverterPlugin{

  private:
    eventPacketAIDAdataConverterPlugin():aidaDataConverterPlugin(eventType,subEventType){

    }

  };
}
#endif // eventPacketAIDAdataConverterPlugin_h__

#ifndef EventSynchronisationDetectorEvents_h__
#define EventSynchronisationDetectorEvents_h__


#include "eudaq/DetectorEvent.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/EventSynchronisationBase.hh"
#include <memory>
#include <queue>
// base class for all Synchronization Plugins
// it is desired to be as modular es possible with this approach.
// first step is to separate the events from different Producers. 
// then it will be recombined to a new event
// The Idea is that the user can define a condition that need to be true to define if an event is sync or not

namespace eudaq{




  class DLLEXPORT syncToDetectorEvents: public SyncBase {
  public:
    
    virtual void makeOutputEvent();
  };

}//namespace eudaq




#endif // EventSynchronisationDetectorEvents_h__

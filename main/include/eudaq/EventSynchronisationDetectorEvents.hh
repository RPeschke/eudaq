#ifndef EventSynchronisationDetectorEvents_h__
#define EventSynchronisationDetectorEvents_h__


#include "eudaq/DetectorEvent.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/EventSynchronisationBase.hh"
#include <memory>
#include <queue>



namespace eudaq{




  class DLLEXPORT syncToDetectorEvents: public SyncBase {
  public:
    syncToDetectorEvents(bool sync);
    virtual void makeDetectorEvent();
    
  private:
        
    virtual void storeCurrentOrder();
    
  };

}//namespace eudaq




#endif // EventSynchronisationDetectorEvents_h__

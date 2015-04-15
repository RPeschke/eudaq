#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/StandardEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Configuration.hh"
// All LCIO-specific parts are put in conditional compilation blocks
// so that the other parts may still be used if LCIO is not available.
#if USE_LCIO
#include "IMPL/LCEventImpl.h"
#include "IMPL/TrackerRawDataImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "lcio.h"
#include "EUTELESCOPE.h"
#include "EUTelRunHeaderImpl.h"
#include "EUTelSetupDescription.h"
#include "EUTelTrackerDataInterfacerImpl.h"
using eutelescope::EUTELESCOPE;
using eutelescope::EUTelTrackerDataInterfacerImpl;
using eutelescope::EUTelGenericSparsePixel;

#endif
#include <iostream>
#include <string>
#include <queue>




namespace eudaq {
  static const int dbg = 0; // 0=off, 1=structure, 2=structure+data
#if USE_LCIO
  bool Collection_createIfNotExist(lcio::LCCollectionVec** zsDataCollection, const lcio::LCEvent & lcioEvent, const char * name){

    bool zsDataCollectionExists = false;
    try
    {
      *zsDataCollection = static_cast<lcio::LCCollectionVec*> (lcioEvent.getCollection(name));
      zsDataCollectionExists = true;
    }
    catch (lcio::DataNotAvailableException& e)
    {
      *zsDataCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERDATA);
    }

    return zsDataCollectionExists;
  }
#endif
 
  static const char* EVENT_TYPE = "sct1";
  static const char* LCIO_collection_name= "zsdata_strip";


  // Declare a new class that inherits from DataConverterPlugin
  class SCTupgradeConverterPlugin_sct1 : public DataConverterPlugin {



  public:

    // This is called once at the beginning of each run.
    // You may extract information from the BORE and/or configuration
    // and store it in member variables to use during the decoding later.
    virtual void Initialize(const Event & bore,
      const Configuration & cnf) {

#ifndef WIN32  //some linux Stuff //$$change
      (void)cnf; // just to suppress a warning about unused parameter cnf
#endif

    }

    // This should return the trigger ID (as provided by the TLU)
    // if it was read out, otherwise it can either return (unsigned)-1,
    // or be left undefined as there is already a default version.
    virtual unsigned GetTriggerID(const Event & ev) const {

      return ev.GetEventNumber();
    }

    virtual void setCurrentTLUEvent(eudaq::Event & ev, eudaq::TLUEvent const & tlu){
     
    }
    // Here, the data from the RawDataEvent is extracted into a StandardEvent.
    // The return value indicates whether the conversion was successful.
    // Again, this is just an example, adapted it for the actual data layout.
    virtual bool GetStandardSubEvent(StandardEvent & sev,
      const Event & ev) const {


      const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);



    
      StandardPlane plane(m_plane_id, EVENT_TYPE);
      

      std::vector<unsigned char> inputVector = rawev.GetBlock(0);

      std::vector<bool> raw_data;
  
      eudaq::uchar2bool(inputVector.data(), inputVector.data() + inputVector.size(), raw_data);

      plane.SetSizeZS(raw_data.size(), 1,0);
      int i = 0;
      for (auto& e:raw_data)
      {
        if (e)
        {
          plane.PushPixel(i, 1, 1);
        }
        ++i;
      }


      plane.SetTLUEvent(GetTriggerID(ev));




      //  std::cout<<"length of plane " <<numberOfEvents_inplane<<std::endl;
      // Add the plane to the StandardEvent	
      sev.AddPlane(plane);
      // Indicate that data was successfully converted
      return true;
    }

  private:

    // The constructor can be private, only one static instance is created
    // The DataConverterPlugin constructor must be passed the event type
    // in order to register this converter for the corresponding conversions
    // Member variables should also be initialized to default values here.
    SCTupgradeConverterPlugin_sct1(): DataConverterPlugin(EVENT_TYPE){}

    const int m_plane_id = 8;
    // The single instance of this converter plugin
    static SCTupgradeConverterPlugin_sct1 m_instance;
  }; // class ExampleConverterPlugin

  // Instantiate the converter plugin instance
  SCTupgradeConverterPlugin_sct1 SCTupgradeConverterPlugin_sct1::m_instance;

} // namespace eudaq

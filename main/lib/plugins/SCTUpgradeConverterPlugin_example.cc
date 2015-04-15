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
 
  static const char* EVENT_TYPE = "sct1";


  // Declare a new class that inherits from DataConverterPlugin
  class SCTupgradeConverterPlugin_sct1 : public DataConverterPlugin {



  public:

    // This is called once at the beginning of each run.
    // You may extract information from the BORE and/or configuration
    // and store it in member variables to use during the decoding later.
    virtual void Initialize(const Event & bore, const Configuration & cnf);

    // This should return the trigger ID (as provided by the TLU)
    // if it was read out, otherwise it can either return (unsigned)-1,
    // or be left undefined as there is already a default version.
    virtual unsigned GetTriggerID(const Event & ev) const;

    virtual void setCurrentTLUEvent(eudaq::Event & ev, eudaq::TLUEvent const & tlu);
    // Here, the data from the RawDataEvent is extracted into a StandardEvent.
    // The return value indicates whether the conversion was successful.
    // Again, this is just an example, adapted it for the actual data layout.
    virtual bool GetStandardSubEvent(StandardEvent & sev, const Event & ev) const;





#if USE_LCIO
    // This is where the conversion to LCIO is done
    void GetLCIORunHeader(lcio::LCRunHeader & header, eudaq::Event const & /*bore*/, eudaq::Configuration const & /*conf*/) const;


    bool GetLCIOSubEvent(lcio::LCEvent & result, const Event & source) const ;
#endif
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


















  void SCTupgradeConverterPlugin_sct1::Initialize(const Event & bore, const Configuration & cnf)
  {
#ifndef WIN32  //some linux Stuff //$$change
    (void)cnf; // just to suppress a warning about unused parameter cnf
#endif
  }

  unsigned SCTupgradeConverterPlugin_sct1::GetTriggerID( const Event & ev ) const
  {
    return ev.GetEventNumber();
  }

  void SCTupgradeConverterPlugin_sct1::setCurrentTLUEvent(eudaq::Event & ev, eudaq::TLUEvent const & tlu)
  {

  }

  bool SCTupgradeConverterPlugin_sct1::GetStandardSubEvent(StandardEvent & sev, const Event & ev) const
  {
    const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);
    
    
    StandardPlane plane(m_plane_id, EVENT_TYPE);
    std::vector<unsigned char> inputVector = rawev.GetBlock(0);
    std::vector<bool> raw_data;

    eudaq::uchar2bool(inputVector.data(), inputVector.data() + inputVector.size(), raw_data);

    plane.SetSizeZS(raw_data.size(), 1, 0);
    int i = 0;
    for (auto& e : raw_data)
    {
      if (e)
      {
        plane.PushPixel(i, 1, 1);
      }
      ++i;
    }


    plane.SetTLUEvent(GetTriggerID(ev));




    // Add the plane to the StandardEvent	
    sev.AddPlane(plane);
    // Indicate that data was successfully converted
    return true;
  }
#ifdef  USE_LCIO


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

  void SCTupgradeConverterPlugin_sct1::GetLCIORunHeader(lcio::LCRunHeader & header, eudaq::Event const & /*bore*/, eudaq::Configuration const & /*conf*/) const {
    eutelescope::EUTelRunHeaderImpl runHeader(&header);
    runHeader.setDAQHWName(EUTELESCOPE::EUDRB); // should be:
    //runHeader.setDAQHWName(EUTELESCOPE::NI);

    // the information below was used by EUTelescope before the
    // introduction of the BUI. Now all these parameters shouldn't be
    // used anymore but they are left here for backward compatibility.

    runHeader.setEUDRBMode("ZS");
    runHeader.setEUDRBDet("SCT");
    runHeader.setNoOfDetector(m_boards);
    std::vector<int> xMin(m_boards, 0), xMax(m_boards, 1280), yMin(m_boards, 0), yMax(m_boards, 4);
    runHeader.setMinX(xMin);
    runHeader.setMaxX(xMax);
    runHeader.setMinY(yMin);
    runHeader.setMaxY(yMax);
  }


  bool SCTupgradeConverterPlugin_sct1::GetLCIOSubEvent(lcio::LCEvent & result, const Event & source) const {


    if (source.IsBORE()) {
      if (dbg > 0) std::cout << "SCTUpgradeConverterPlugin::GetLCIOSubEvent BORE " << std::endl;
      // shouldn't happen
      return true;
    }
    else if (source.IsEORE()) {
      if (dbg > 0) std::cout << "SCTUpgradeConverterPlugin::GetLCIOSubEvent EORE " << std::endl;
      // nothing to do
      return true;
    }
    // If we get here it must be a data event

    if (dbg > 0) std::cout << "SCTUpgradeConverterPlugin::GetLCIOSubEvent data " << std::endl;
    result.parameters().setValue(eutelescope::EUTELESCOPE::EVENTTYPE, eutelescope::kDE);


    LCCollectionVec *zsDataCollection = nullptr;
    auto zsDataCollectionExists = Collection_createIfNotExist(&zsDataCollection, result, LCIO_collection_name);



    StandardEvent tmp_evt;
    GetStandardSubEvent(tmp_evt, source);
    auto plane = tmp_evt.GetPlane(0);


    // set the proper cell encoder
    auto  zsDataEncoder = CellIDEncoder<TrackerDataImpl>(eutelescope::EUTELESCOPE::ZSDATADEFAULTENCODING, zsDataCollection);
    zsDataEncoder["sensorID"] = plane.ID();
    zsDataEncoder["sparsePixelType"] = eutelescope::kEUTelGenericSparsePixel;


    // prepare a new TrackerData for the ZS data
    auto zsFrame = std::unique_ptr<lcio::TrackerDataImpl>(new lcio::TrackerDataImpl());
    zsDataEncoder.setCellID(zsFrame.get());


    ConvertPlaneToLCIOGenericPixel(plane, *zsFrame);

    // perfect! Now add the TrackerData to the collection
    zsDataCollection->push_back(zsFrame.release());


    if (!zsDataCollectionExists){
      if (zsDataCollection->size() != 0)
        result.addCollection(zsDataCollection, LCIO_collection_name);
      else
        delete zsDataCollection; // clean up if not storing the collection here
    }

    return true;
  }

#endif //  USE_LCIO
} // namespace eudaq

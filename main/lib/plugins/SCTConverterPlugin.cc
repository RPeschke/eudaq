#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/StandardEvent.hh"
#include "eudaq/Utils.hh"

// All LCIO-specific parts are put in conditional compilation blocks
// so that the other parts may still be used if LCIO is not available.
#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "lcio.h"
#endif
#ifdef USE_EUDAQ2_VERSION
#include "eudaq/Processor.hh"
#endif // USE_EUDAQ2_VERSION

#include "eudaq/PluginManager.hh"

#include "eudaq/SCT_defs.hh"

namespace eudaq {

  // The event type for which this converter plugin will be registered
  // Modify this to match your actual event type (from the Producer)
  static const char* EVENT_TYPE_ITS_ABC = "ITS_ABC";
  static const int PlaneID = 8;   
  namespace sct{
    std::string TDC_L0ID(){ return "TDC.L0ID"; }
    std::string TLU_TLUID(){ return "TLU.TLUID"; }
    std::string TDC_data(){ return "TDC.data"; }
    std::string TLU_L0ID(){ return "TLU.L0ID"; }
    std::string Timestamp_data(){ return "Timestamp.data"; }
    std::string Timestamp_L0ID(){ return "Timestamp.L0ID"; }
    std::string Event_L0ID(){ return "Event.L0ID"; }
    std::string Event_BCID(){ return "Event.BCID"; }
  }

  using namespace sct;
  // Declare a new class that inherits from DataConverterPlugin
  class SCTConverterPlugin_ITS_ABC : public DataConverterPlugin {

    public:
	
      // This is called once at the beginning of each run.
      // You may extract information from the BORE and/or configuration
      // and store it in member variables to use during the decoding later.
      virtual void Initialize(const Event & bore,  const Configuration & cnf) {

      }

      virtual int IsSyncWithTLU(eudaq::Event const & ev, const eudaq::Event  & tluEvent) const {
        unsigned triggerID = GetTriggerID(ev);
        auto tlu_triggerID = tluEvent.GetEventNumber();
        return compareTLU2DUT(tlu_triggerID, triggerID+1);
      }


      virtual bool GetStandardSubEvent(StandardEvent & sev, const Event & ev) const {
        if (ev.IsBORE())
        {
          return true;
        }
        
        auto raw = dynamic_cast<const RawDataEvent*>(&ev);
        if (!raw)
        {
          return false;
        }


        auto block=raw->GetBlock(0);
        
        std::vector<bool> channels;
        eudaq::uchar2bool(block.data(), block.data() + block.size(), channels);

        StandardPlane plane(PlaneID, EVENT_TYPE_ITS_ABC);
        plane.SetSizeZS(channels.size(), 1,0);
        unsigned x = 0;
        unsigned y = 0;
        for(auto & e:channels){
          ++x;
          if (e==true)
          {
            plane.PushPixel(x,y,1);
          }

        }
        sev.AddPlane(plane);
        return true;
      }
#if USE_LCIO
      // This is where the conversion to LCIO is done
    virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
        return 0;
      }
#endif

      static SCTConverterPlugin_ITS_ABC m_instance;
    private:

      SCTConverterPlugin_ITS_ABC(): DataConverterPlugin(EVENT_TYPE_ITS_ABC) {}


   
      // The single instance of this converter plugin
  }; // class SCTConverterPlugin

  // Instantiate the converter plugin instance
  SCTConverterPlugin_ITS_ABC SCTConverterPlugin_ITS_ABC::m_instance;








  using TriggerData_t = uint64_t;

  // The event type for which this converter plugin will be registered
  // Modify this to match your actual event type (from the Producer)
  static const char* EVENT_TYPE_ITS_TTC = "ITS_TTC";

  // Declare a new class that inherits from DataConverterPlugin
  class SCTConverterPlugin_ITS_TTC : public DataConverterPlugin {

  public:
   
    
    virtual void Initialize(const Event & bore, const Configuration & cnf) {

    }
    virtual int IsSyncWithTLU(eudaq::Event const & ev, const eudaq::Event  & tluEvent) const {
      unsigned triggerID = GetTriggerID(ev);
      auto tlu_triggerID = tluEvent.GetEventNumber();
      return compareTLU2DUT(tlu_triggerID, triggerID + 1);
    }
 

    // Here, the data from the RawDataEvent is extracted into a StandardEvent.
    // The return value indicates whether the conversion was successful.
    // Again, this is just an example, adapted it for the actual data layout.
    virtual bool GetStandardSubEvent(StandardEvent & sev,
      const Event & ev) const {

      if (ev.IsBORE())
      {
        return true;
      }


      auto raw = dynamic_cast<const RawDataEvent*>(&ev);
      if (!raw)
      {
        return false;
      }
      
      auto block = raw->GetBlock(0);
      
      ProcessTTC(block, sev);

      return true;
    }
    template<typename T>
   static void ProcessTTC(const std::vector<unsigned char>& block, T& sev){

      size_t size_of_TTC = block.size() / sizeof(TriggerData_t);
      const TriggerData_t *TTC = nullptr;
      if (size_of_TTC)
      {
        TTC = reinterpret_cast<const TriggerData_t*>(block.data());
      }
      size_t j = 0;
      // sev.SetTag(std::string("triggerData_") + to_string(j), to_hex(TTC[j], 16));
     // sev.SetTag("triggerData_0", to_hex(TTC[0], 16));
      for (size_t i = 0; i < size_of_TTC; ++i)
      {
       
        uint64_t data = TTC[i];
        switch (data >> 60) {
        case 0xd:
          ProcessTLU_data(data, sev);
          break;
        case 0xe:
          ProcessTDC_data(data, sev);
          break;

        case 0xf:
          ProcessTimeStamp_data(data, sev);
          break;
        default:
          EUDAQ_THROW("unknown data type");
          break;
        }
      }

    }

    template<typename T>
    static  void ProcessTLU_data(uint64_t data, T & sev) {
      uint32_t hsioID = (uint32_t)(data >> 40) & 0xffff;
      uint64_t TLUID = data & 0xffff;

      sev.SetTag(TLU_TLUID(), to_hex(TLUID, 4));
      sev.SetTag(TDC_data(), to_hex(hsioID, 4));

    }
    template<typename T>
    static   void ProcessTDC_data(uint64_t data, T & sev) {

      uint32_t L0ID = (uint32_t)(data >> 40) & 0xffff;
      uint64_t TDC = data & 0xfffff;
      sev.SetTag(TDC_data(), to_hex(TDC, 5));
      sev.SetTag(TDC_L0ID(), to_hex(L0ID, 4));
    }
    template<typename T>
    static void ProcessTimeStamp_data(uint64_t data, T & sev) {
      
       
        uint64_t  timestamp = data & 0x000000ffffffffffULL;
        uint32_t L0ID = (uint32_t)(data >> 40) & 0xffff;
        sev.SetTag(Timestamp_data(), to_hex(timestamp, 10));
        sev.SetTag(Timestamp_L0ID(), to_hex(L0ID ,4));
     

    }
#if USE_LCIO
    // This is where the conversion to LCIO is done
    virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
      return 0;
    }
#endif
    static SCTConverterPlugin_ITS_TTC m_instance;

  private:

    // The constructor can be private, only one static instance is created
    // The DataConverterPlugin constructor must be passed the event type
    // in order to register this converter for the corresponding conversions
    // Member variables should also be initialized to default values here.
    SCTConverterPlugin_ITS_TTC() : DataConverterPlugin(EVENT_TYPE_ITS_TTC) {}

    // Information extracted in Initialize() can be stored here:

    // The single instance of this converter plugin
  }; // class SCTConverterPlugin

  // Instantiate the converter plugin instance
  SCTConverterPlugin_ITS_TTC SCTConverterPlugin_ITS_TTC::m_instance;











  // The event type for which this converter plugin will be registered
  // Modify this to match your actual event type (from the Producer)
  static const char* EVENT_TYPE_SCT = "SCT";

  // Declare a new class that inherits from DataConverterPlugin
  class SCTConverterPlugin: public DataConverterPlugin {

  public:


    virtual void Initialize(const Event & bore, const Configuration & cnf) {

    }
    virtual int IsSyncWithTLU(eudaq::Event const & ev, const eudaq::Event  & tluEvent) const {
      unsigned triggerID = GetTriggerID(ev);
      auto tlu_triggerID = tluEvent.GetEventNumber();
      return compareTLU2DUT(tlu_triggerID, triggerID + 1);
    }


    // Here, the data from the RawDataEvent is extracted into a StandardEvent.
    // The return value indicates whether the conversion was successful.
    // Again, this is just an example, adapted it for the actual data layout.
    virtual bool GetStandardSubEvent(StandardEvent & sev,
      const Event & ev) const {

      if (ev.IsBORE())
      {
        return true;
      }

      auto raw = dynamic_cast<const RawDataEvent*>(&ev);
      if (!raw)
      {
        return false;
      }


      auto block=raw->GetBlock(0);

      std::vector<bool> channels;
      eudaq::uchar2bool(block.data(), block.data() + block.size(), channels);

      StandardPlane plane(PlaneID, EVENT_TYPE_ITS_ABC);
      plane.SetSizeZS(channels.size(), 1, 0);
      unsigned x = 0;
      unsigned y = 0;
      for (auto & e : channels){
        ++x;
        if (e == true)
        {
          plane.PushPixel(x, y, 1);
        }

      }
      sev.AddPlane(plane);

      sev.SetTag(TDC_data(), ev.GetTag(TDC_data(), ""));
      sev.SetTag(TDC_L0ID(), ev.GetTag(TDC_L0ID(), ""));
      sev.SetTag(TLU_TLUID(), ev.GetTag(TLU_TLUID(), ""));
      sev.SetTag(TDC_data(), ev.GetTag(TDC_data(), ""));

      sev.SetTag(Timestamp_data(), ev.GetTag(Timestamp_data(), ""));
      sev.SetTag(Timestamp_L0ID(), ev.GetTag(Timestamp_L0ID(), ""));

      return true;
    }
#if USE_LCIO
    // This is where the conversion to LCIO is done
    virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
      return 0;
    }
#endif

    static SCTConverterPlugin m_instance;

  private:

    // The constructor can be private, only one static instance is created
    // The DataConverterPlugin constructor must be passed the event type
    // in order to register this converter for the corresponding conversions
    // Member variables should also be initialized to default values here.
    SCTConverterPlugin() : DataConverterPlugin(EVENT_TYPE_SCT) {}

    // Information extracted in Initialize() can be stored here:

    // The single instance of this converter plugin
  }; // class SCTConverterPlugin

  // Instantiate the converter plugin instance
  SCTConverterPlugin SCTConverterPlugin::m_instance;
#ifdef USE_EUDAQ2_VERSION



  class mergeITSDAQStreams : public Processor{
  public:
    mergeITSDAQStreams(Parameter_ref conf) :Processor(conf){}
    virtual ReturnParam ProcessorEvent(event_sp ev) override{
      if (!ev)
      {
        return ProcessorBase::stop;
      }
      if (ev->IsBORE())
      {
        return ProcessNext(ev);
      }
      if (ev->IsEORE())
      {
        return ProcessNext(ev);
      }
      auto det = dynamic_cast<DetectorEvent*>(ev.get());

      if (!det)
      {
        return ProcessorBase::ret_error;
      }

      event_sp abc,ttc;
      std::vector<event_sp> dummy;
      for (int i = 0; i < det->NumEvents(); ++i)
      {
        auto currentEvent = det->GetEventPtr(i);

        if (PluginManager::getUniqueIdentifier(*currentEvent) == SCTConverterPlugin_ITS_TTC::m_instance.getUniqueIdentifier(*ev))
        {
          ttc = currentEvent;

        }else if (PluginManager::getUniqueIdentifier(*currentEvent) == SCTConverterPlugin_ITS_ABC::m_instance.getUniqueIdentifier(*ev))
        {
          abc = currentEvent;
        }
        else{
          dummy.push_back(currentEvent);
        }
      }


      auto rawTTC = dynamic_cast<RawDataEvent*>(ttc.get());

      if (!rawTTC)
      {
        return ProcessorBase::ret_error;
      }

      det->clearEvents();
      SCTConverterPlugin_ITS_TTC::ProcessTTC( rawTTC->GetBlock(0), *abc);
      det->AddEvent(abc);
      for (auto& e:dummy)
      {
        det->AddEvent(e);
      }
      return ProcessNext(ev);
    }

  };

  std::string sct::mergeITSDAQStreamsName(){
    return "mergeITSDAQStreams";
  }



  RegisterProcessor(mergeITSDAQStreams, sct::mergeITSDAQStreamsName());

  class SCT_COMPARE : public Processor{
  public:
    SCT_COMPARE(Parameter_ref conf) :Processor(conf){}
    virtual ReturnParam ProcessorEvent(event_sp ev) override{
      bool same = true;
      if (ev->IsBORE())
      {
        return ProcessNext(ev);
      }
      if (ev->IsEORE())
      {
        return ProcessNext(ev);
      }

      if (m_ev != ev->GetEventNumber())
      {
        // new event 
        m_first = ev;
        m_ev = ev->GetEventNumber();
        return ProcessorBase::sucess;
      }
      m_second = ev;
      if (ev->GetEventNumber()==5)
      {
        same = false;
      }

      auto raw1 = dynamic_cast<RawDataEvent*>(dynamic_cast<DetectorEvent*>(m_first.get())->GetEvent(0));
      auto raw2 = dynamic_cast<RawDataEvent*>(dynamic_cast<DetectorEvent*>(m_second.get())->GetEvent(0));


      auto block1 = raw1->GetBlock(0);
      auto block2 = raw2->GetBlock(0);

      if (block1.size()!=block2.size())
      {
//       same = false;
//       std::cout << "different block sizes \n block1: " << block1.size() << "\n block2: " << block2.size() << std::endl;
      }

      auto min_s = min(block1.size(), block2.size());
      std::vector<size_t> errors;
      for (size_t i = 0; i < min_s; i++)
      {
        if (block1[i]!=block2[2])
        {
          same = false;
          errors.push_back(i);
          std::cout << "block difference at: " << i << std::endl;
        }

      }
      


      if (Is_unequal(raw1, raw2, TDC_L0ID())
        ||
        Is_unequal(raw1, raw2, TLU_TLUID())
        ||
        Is_unequal(raw1, raw2, TDC_data())
        ||
        Is_unequal(raw1, raw2, TDC_data())
        ||
        Is_unequal(raw1, raw2, Timestamp_data())
        ||
        Is_unequal(raw1, raw2, Timestamp_L0ID()))
      {

        same = false;
      }
     
     
      

      if (same==true)
      {
        return ProcessorBase::sucess;
      }
      

      auto ret=ProcessNext(m_first);

      if (ret!=ProcessorBase::sucess)
      {
        return ret;
      }

      return ProcessNext(m_second);
    }

    virtual void initialize(Configuration_ref conf)override{

      m_first.reset();
      m_second.reset();
      m_ev=121234;

    }

    event_sp m_first, m_second;
    unsigned m_ev;
    static bool Is_unequal(RawDataEvent* raw1, RawDataEvent* raw2, const std::string & name){
      if (hex2uint_64(raw1->GetTag(name, "0")) != hex2uint_64(raw2->GetTag(name, "0"))){
        std::cout << "block difference at: " << name << " " << raw1->GetTag(name, "") << " "<< raw2->GetTag(name, "") << std::endl;
        return true;
      }
      return false;
    }
  };

  std::string sct::SCT_COMPARE_Name(){
    return "SCT_COMPARE";
  }



  RegisterProcessor(SCT_COMPARE, sct::SCT_COMPARE_Name());
#endif // USE_EUDAQ2_VERSION

} // namespace eudaq

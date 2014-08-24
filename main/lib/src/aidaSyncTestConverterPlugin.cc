#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/StandardEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/AidaPacket.hh"
#include "eudaq/PluginManager.hh"
// All LCIO-specific parts are put in conditional compilation blocks
// so that the other parts may still be used if LCIO is not available.


namespace eudaq {
  typedef eudaq::AidaPacket containerT;
  // The event type for which this converter plugin will be registered
  // Modify this to match your actual event type (from the Producer)
  static const char* EVENT_TYPE = "slowP";

  // Declare a new class that inherits from DataConverterPlugin
  class aidaSyncTestConverterPlugin : public DataConverterPlugin<AidaPacket> {

    public:

      // This is called once at the beginning of each run.
      // You may extract information from the BORE and/or configuration
      // and store it in member variables to use during the decoding later.
      virtual void Initialize(const AidaPacket & bore,
          const Configuration & cnf) {
        m_exampleparam = bore.GetTag("EXAMPLE", 0);
#ifndef WIN32  //some linux Stuff //$$change
		(void)cnf; // just to suppress a warning about unused parameter cnf
#endif
        
      }

      // This should return the trigger ID (as provided by the TLU)
      // if it was read out, otherwise it can either return (unsigned)-1,
      // or be left undefined as there is already a default version.
      virtual unsigned GetTriggerID(const AidaPacket & ev) const {

        return (unsigned)-1;
      }
      virtual int IsSyncWithTLU(AidaPacket const & ev, AidaPacket const & tlu) const {
        // dummy comparator. it is just checking if the event numbers are the same.

        //auto triggerID=ev.GetEventNumber();
  
        auto tlu_triggerID = PluginManager<containerT>::GetTriggerID(tlu);
        int min_ret = 2;
        int max_ret = -2;
        int ret = 5;
        uint64_t triggerID = 0;
        
        for (size_t i = m_triggerIndexCounter; i < ev.GetMetaData().Size();++i)
        {
          std::cout << "Event  " << AidaPacket::type2str(ev.GetPacketType()) << "  time stamp " << ev.GetEventNumber(i) << std::endl;
          triggerID = ev.GetEventNumber(i);
          ret = compareTLU2DUT(tlu_triggerID, triggerID);
          if (ret == Event_IS_Sync)
          {
            m_triggerIndexCounter = i+1;
            return Event_IS_Sync;
          }

          max_ret = max(max_ret, ret);
          min_ret = min(min_ret, ret);
        }
        m_triggerIndexCounter = 0;
        if (max_ret==Event_IS_EARLY && min_ret==Event_IS_EARLY)
        {
          return Event_IS_EARLY;
        }

        return Event_IS_LATE;  // this value is returned either when min or max is late and non of the triggers is sync. it means either that this trigger was missed or that the event is in general late 



      }

      // Here, the data from the RawDataEvent is extracted into a StandardEvent.
      // The return value indicates whether the conversion was successful.
      // Again, this is just an example, adapted it for the actual data layout.
      virtual bool GetStandardSubEvent(StandardEvent & sev,
        const AidaPacket & ev) const {

        return true;
      }

#if USE_LCIO
      // This is where the conversion to LCIO is done
    virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
        return 0;
      }
#endif

    private:

      // The constructor can be private, only one static instance is created
      // The DataConverterPlugin constructor must be passed the event type
      // in order to register this converter for the corresponding conversions
      // Member variables should also be initialized to default values here.
      aidaSyncTestConverterPlugin()
        : DataConverterPlugin(AidaPacket::str2type(EVENT_TYPE),1), m_exampleparam(0)
      {}

      // Information extracted in Initialize() can be stored here:
      unsigned m_exampleparam;
      mutable size_t m_triggerIndexCounter = 0;
      // The single instance of this converter plugin
      static aidaSyncTestConverterPlugin m_instance;
  }; // class ExampleConverterPlugin

  // Instantiate the converter plugin instance
  aidaSyncTestConverterPlugin aidaSyncTestConverterPlugin::m_instance;












    // The event type for which this converter plugin will be registered
    // Modify this to match your actual event type (from the Producer)
    static const char* EVENT_TYPE1 = "fastP";

    // Declare a new class that inherits from DataConverterPlugin
    class aidaSyncTestConverterPlugin1 : public DataConverterPlugin < AidaPacket > {

    public:

      // This is called once at the beginning of each run.
      // You may extract information from the BORE and/or configuration
      // and store it in member variables to use during the decoding later.
      virtual void Initialize(const AidaPacket & bore,
        const Configuration & cnf) {
        m_exampleparam = bore.GetTag("EXAMPLE", 0);
#ifndef WIN32  //some linux Stuff //$$change
        (void)cnf; // just to suppress a warning about unused parameter cnf
#endif

      }


      virtual int IsSyncWithTLU(AidaPacket const & ev, AidaPacket const & tlu) const {
        // dummy comparator. it is just checking if the event numbers are the same.

        //auto triggerID=ev.GetEventNumber();
        unsigned triggerID = ev.GetEventNumber();
        auto tlu_triggerID = PluginManager<containerT>::GetTriggerID(tlu);
        
        return compareTLU2DUT(tlu_triggerID, triggerID);
      }

      // Here, the data from the RawDataEvent is extracted into a StandardEvent.
      // The return value indicates whether the conversion was successful.
      // Again, this is just an example, adapted it for the actual data layout.
      virtual bool GetStandardSubEvent(StandardEvent & sev,
        const AidaPacket & ev) const {

        return true;
      }

#if USE_LCIO
      // This is where the conversion to LCIO is done
      virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
        return 0;
      }
#endif

    private:

      // The constructor can be private, only one static instance is created
      // The DataConverterPlugin constructor must be passed the event type
      // in order to register this converter for the corresponding conversions
      // Member variables should also be initialized to default values here.
      aidaSyncTestConverterPlugin1()
        : DataConverterPlugin(AidaPacket::str2type(EVENT_TYPE1), 2), m_exampleparam(0)
      {}

      // Information extracted in Initialize() can be stored here:
      unsigned m_exampleparam;

      // The single instance of this converter plugin
      static aidaSyncTestConverterPlugin1 m_instance;
    }; // class ExampleConverterPlugin

    // Instantiate the converter plugin instance
    aidaSyncTestConverterPlugin1 aidaSyncTestConverterPlugin1::m_instance;



    // The event type for which this converter plugin will be registered
    
    static const char* EVENT_TYPE2 = "multiFrame";

    // Declare a new class that inherits from DataConverterPlugin
    class multiFrameDataConverter : public DataConverterPlugin < AidaPacket > {

    public:

      // This is called once at the beginning of each run.
      // You may extract information from the BORE and/or configuration
      // and store it in member variables to use during the decoding later.
      virtual void Initialize(const AidaPacket & bore,
        const Configuration & cnf) {
        m_exampleparam = bore.GetTag("EXAMPLE", 0);
#ifndef WIN32  //some linux Stuff //$$change
        (void)cnf; // just to suppress a warning about unused parameter cnf
#endif

      }

      // This should return the trigger ID (as provided by the TLU)
      // if it was read out, otherwise it can either return (unsigned)-1,
      // or be left undefined as there is already a default version.
      virtual unsigned GetTriggerID(const AidaPacket & ev) const {

        return ev.GetEventNumber(m_elenentNumerator);
      }
      virtual int IsSyncWithTLU(AidaPacket const & ev, AidaPacket const & tlu) const {
        // dummy comparator. it is just checking if the event numbers are the same.

        //auto triggerID=ev.GetEventNumber();
        
        unsigned triggerID = ev.GetEventNumber(m_elenentNumerator);
        auto tlu_triggerID = PluginManager<AidaPacket>::GetTriggerID(tlu);
        return compareTLU2DUT(tlu_triggerID, triggerID);
      }

      // Here, the data from the RawDataEvent is extracted into a StandardEvent.
      // The return value indicates whether the conversion was successful.
      // Again, this is just an example, adapted it for the actual data layout.
      virtual bool GetStandardSubEvent(StandardEvent & sev,
        const AidaPacket & ev) const {

        return true;
      }
      virtual size_t getNumberOfElemts(const containerT &ev){ return ev.GetMetaData().Size(); }

#if USE_LCIO
      // This is where the conversion to LCIO is done
      virtual lcio::LCEvent * GetLCIOEvent(const Event * /*ev*/) const {
        return 0;
      }
#endif

    private:

      // The constructor can be private, only one static instance is created
      // The DataConverterPlugin constructor must be passed the event type
      // in order to register this converter for the corresponding conversions
      // Member variables should also be initialized to default values here.
      multiFrameDataConverter()
        : DataConverterPlugin(AidaPacket::str2type(EVENT_TYPE2), 3), m_exampleparam(0)
      {}

      // Information extracted in Initialize() can be stored here:
      unsigned m_exampleparam;

      // The single instance of this converter plugin
      static multiFrameDataConverter m_instance;
    }; // class ExampleConverterPlugin

    // Instantiate the converter plugin instance
    multiFrameDataConverter multiFrameDataConverter::m_instance;


} // namespace eudaq

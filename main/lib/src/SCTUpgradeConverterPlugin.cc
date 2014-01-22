#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/StandardEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Configuration.hh"
// All LCIO-specific parts are put in conditional compilation blocks
// so that the other parts may still be used if LCIO is not available.
#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "lcio.h"
#endif
#include <iostream>
#include <string>

#define EVENTHEADERSIZE 14
#define MODULEHEADERSIZE 3
#define STREAMHEADERSIZE 3
#define STREAMESIZE 160
#define TOTALHEADERSIZE (EVENTHEADERSIZE+MODULEHEADERSIZE+STREAMHEADERSIZE)
#define STARTSTREAM0 TOTALHEADERSIZE
#define ENDSTREAM0 (STARTSTREAM0+STREAMESIZE)
#define STARTSTREAM1 (TOTALHEADERSIZE+STREAMHEADERSIZE+STREAMESIZE)
#define ENDSTREAM1 (STARTSTREAM1+STREAMESIZE)
#define TOTALMODULSIZE (MODULEHEADERSIZE+2*STREAMHEADERSIZE+2*STREAMESIZE)
#define STREAMSTART(streamNr) (TOTALHEADERSIZE+streamNr*(STREAMHEADERSIZE+STREAMESIZE))
#define STREAMEND(streamNr) (STREAMSTART(streamNr)+STREAMESIZE)


#define TLU_chlocks_per_mirco_secound 384066


void uchar2bool(const std::vector<unsigned char>& in,int lOffset,int hOffset, std::vector<bool>& out){
	for (auto i=in.begin()+lOffset;i!=in.begin()+hOffset;++i)
	{
		for(int j=0;j<8;++j){
			out.push_back((*i)&(1<<j));
		}
	}


}



namespace eudaq {

	void puschDataInStandartPlane(const std::vector<unsigned char>& inputVector,int moduleNr, StandardPlane& plane){
		int y_pos=(moduleNr-1)*2+1;

		for (int streamNr=0;streamNr<2;++streamNr)
		{
		
		std::vector<bool> outputStream0;




		uchar2bool(inputVector,
			STREAMSTART(streamNr) +(moduleNr-1)*TOTALMODULSIZE,
			STREAMEND(streamNr)   +(moduleNr-1)*TOTALMODULSIZE,
			outputStream0);

		for (size_t i=0; i<outputStream0.size();++i)
		{

			if (outputStream0.at(i))
			{
				plane.PushPixel(i,y_pos+streamNr,1);
			}


		}
		
		}
	}

  // The event type for which this converter plugin will be registered
  // Modify this to match your actual event type (from the Producer)
  static const char* EVENT_TYPE = "SCTupgrade";

  // Declare a new class that inherits from DataConverterPlugin
  class SCTupgradeConverterPlugin : public DataConverterPlugin {

	 

    public:

      // This is called once at the beginning of each run.
      // You may extract information from the BORE and/or configuration
      // and store it in member variables to use during the decoding later.
      virtual void Initialize(const Event & bore,
          const Configuration & cnf) {
        m_exampleparam = bore.GetTag("SCTupgrade", 0);
		auto longdelay=cnf.Get("timeDelay","0");
		cnf.SetSection("EventStruct");

		auto configFile_long_time=cnf.Get("LongBusyTime","0");
	//	std::cout<<" longdelay "<< longdelay<<std::endl;
		unsigned long long longPause_time_from_command_line=0;
		
		try{

			longPause_time_from_command_line=TLU_chlocks_per_mirco_secound*std::stoull(longdelay);
			longPause_time=TLU_chlocks_per_mirco_secound*std::stoull(configFile_long_time);

		}
		catch(...)
		{

			std::string errorMsg="error in SCT Upgrade plugin \n unable to convert " + to_string(longdelay) +"to unsigned long long";
			EUDAQ_THROW(errorMsg);
		}
		if (longPause_time_from_command_line>0)
		{
			longPause_time=longPause_time_from_command_line;
		}
#ifndef WIN32  //some linux Stuff //$$change
		(void)cnf; // just to suppress a warning about unused parameter cnf
#endif
        
      }

      // This should return the trigger ID (as provided by the TLU)
      // if it was read out, otherwise it can either return (unsigned)-1,
      // or be left undefined as there is already a default version.
      virtual unsigned GetTriggerID(const Event & ev) const {
        static const unsigned TRIGGER_OFFSET = 8;
        // Make sure the event is of class RawDataEvent
        if (const RawDataEvent * rev = dynamic_cast<const RawDataEvent *> (&ev)) {
          // This is just an example, modified it to suit your raw data format
          // Make sure we have at least one block of data, and it is large enough

            return rev->GetEventNumber();
          
        }
        // If we are unable to extract the Trigger ID, signal with (unsigned)-1
        return (unsigned)-1;
      }

	   virtual int IsSyncWithTLU(eudaq::Event const & ev,eudaq::TLUEvent const & tlu) const {
		   int returnValue=Event_IS_Sync;
		   unsigned long long tluTime=tlu.GetTimestamp();
		   

		   unsigned long long sctTime=TLU_chlocks_per_mirco_secound*ev.GetTimestamp();

		   if (tluTime-last_TLU_time<longPause_time&&sctTime-Last_DUT_Time<longPause_time)
		   {
			   returnValue=Event_IS_Sync;
			   last_TLU_time=tluTime;
			   Last_DUT_Time=sctTime;
		   }else if (tluTime-last_TLU_time>=longPause_time
			          &&
					 sctTime-Last_DUT_Time<longPause_time)
		   {
			    Last_DUT_Time=sctTime;
			   returnValue=Event_IS_LATE;
			//   std::cout<<"Event_IS_LATE "<<std::endl;
		   }else if (tluTime-last_TLU_time<longPause_time
			   &&
			   sctTime-Last_DUT_Time>=longPause_time)
		   {
			    last_TLU_time=tluTime;
			   returnValue=Event_IS_EARLY;
		//	   std::cout<<"Event_IS_EARLY "<<std::endl;
		   }else if (tluTime-last_TLU_time>=longPause_time
			   &&
			   sctTime-Last_DUT_Time>=longPause_time)
		   {
			   returnValue=Event_IS_Sync;
			   last_TLU_time=tluTime;
			   Last_DUT_Time=sctTime;
		   }
		   
		 
		  


		   return returnValue;
	   
	   
	   }
      // Here, the data from the RawDataEvent is extracted into a StandardEvent.
      // The return value indicates whether the conversion was successful.
      // Again, this is just an example, adapted it for the actual data layout.
      virtual bool GetStandardSubEvent(StandardEvent & sev,
          const Event & ev) const {
        // If the event type is used for different sensors
        // they can be differentiated here
			
        std::string sensortype = "SCT";
        // Create a StandardPlane representing one sensor plane
        int id = 8;
       
        // Set the number of pixels
		 const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);
		 
		 sev.SetTag("DUT_time",rawev.GetTimestamp());
		 int noModules=(rawev.GetBlock(0).size()-EVENTHEADERSIZE)/TOTALMODULSIZE;
		 
		 std::vector<unsigned char> inputVector=rawev.GetBlock(0);
		 int y_pos=0;
		 StandardPlane plane(id, EVENT_TYPE, sensortype);
		 int width = 1280, height = noModules*2;
		 plane.SetSizeRaw(width, height);

		 for (size_t k=1;k<=noModules;++k)
		 {
	
			 puschDataInStandartPlane(inputVector,k,plane);
			
		}
	 
		 // Set the trigger ID
		 plane.SetTLUEvent(GetTriggerID(ev));
		 
		 // Add the plane to the StandardEvent
		 sev.AddPlane(plane);
       // Indicate that data was successfully converted
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
      SCTupgradeConverterPlugin()
        : DataConverterPlugin(EVENT_TYPE), m_exampleparam(0),last_TLU_time(0),Last_DUT_Time(0),longPause_time(0)
      {}

      // Information extracted in Initialize() can be stored here:
      unsigned m_exampleparam;
	 mutable  unsigned long long last_TLU_time,Last_DUT_Time,longPause_time;
      // The single instance of this converter plugin
      static SCTupgradeConverterPlugin m_instance;
  }; // class ExampleConverterPlugin

  // Instantiate the converter plugin instance
  SCTupgradeConverterPlugin SCTupgradeConverterPlugin::m_instance;

} // namespace eudaq

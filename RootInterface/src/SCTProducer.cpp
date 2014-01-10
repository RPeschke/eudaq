#include "fileReader.h"
//#include "..\..\Stavlet_gui\globalvars.h"


// void StartTestbeamProducer(){
// 
// 
// 	std::cout<<"bla bla"<<std::endl;
// }



#include "eudaq/Producer.hh"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Timer.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/ExampleHardware.hh"
#include <iostream>
#include <ostream>
#include <vector>
#include "eudaq/EudaqThread.hh"
#include <time.h>
#include "../inc/SCTProducer.h"

#include "eudaq/Event.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/Producer.hh"
#include <string>
 

void * workerthread_thread(void * arg);
static const std::string EVENT_TYPE = "SCTupgrade";

class SCTProducer::Producer_PImpl : public eudaq::Producer {
public:
	Producer_PImpl(const std::string & name, const std::string & runcontrol,SCTProducer* InterfaceBase): eudaq::Producer(name, runcontrol),
		m_run(0), m_ev(0), stopping(false), done(false),started(false),isConfigured(false),SCT_thread(),m_eventPending(0),m_interface(InterfaceBase) {
			std::cout<< "hallo from sct producer"<<std::endl;
	}
		// This gets called whenever the DAQ is configured
	virtual void OnConfigure(const eudaq::Configuration & config)  {
		m_config=config;
		isConfigured=true;
		std::cout << "Configuring: " << getConfiguration().Name() << std::endl;
		
		m_interface->send_onConfigure();
		// Do any configuration of the hardware here
		// Configuration file values are accessible as config.Get(name, default)


		// At the end, set the status that will be displayed in the Run Control.
		SetStatus(eudaq::Status::LVL_OK, "Configured (" + config.Name() + ")");
	}
	eudaq::Configuration& getConfiguration(){
			return m_config;
	}
	// This gets called whenever a new run is started
	// It receives the new run number as a parameter
	void OnStartRun(unsigned param) {
		// version 0.1 Susanne from LatencyScan.cpp
		std::cout<<"virtual void OnStartRun(unsigned param)"<<std::endl;
		
		m_run =param;
		m_ev=0;
		//	send2TST_setEventNo(0);
		started=true;
		//	send2TST_setTelStatus(1);

		//e->tel_status=1;
		startTime_=clock();

		SCT_thread.start(workerthread_thread,m_interface);

		// It must send a BORE to the Data Collector
		eudaq::RawDataEvent bore(eudaq::RawDataEvent::BORE(EVENT_TYPE, m_run));
		// You can set tags on the BORE that will be saved in the data file
		// and can be used later to help decoding
		bore.SetTag("EXAMPLE", eudaq::to_string(m_exampleparam));
		// Send the event to the Data Collector
		SendEvent(bore);

		// At the end, set the status that will be displayed in the Run Control.
		SetStatus(eudaq::Status::LVL_OK, "Running");

	}
	// This gets called whenever a run is stopped
void OnStopRun() {
		std::cerr << "Stopping Run " << std::endl;
		m_interface->send_onStop();
		// Set a flag to signal to the polling loop that the run is over
		stopping = true;

		// wait until all events have been read out from the hardware
		while (stopping) {
			eudaq::mSleep(20);
		}
		std::cerr<<m_ev << " Events Processed" << std::endl;
		// Send an EORE after all the real events have been sent
		// You can also set tags on it (as with the BORE) if necessary
		SendEvent(eudaq::RawDataEvent::EORE("Test", m_run, ++m_ev));
	}
// This gets called when the Run Control is terminating,
// we should also exit.
void OnTerminate() {
	std::cout << "Terminating..." << std::endl;
	
	done = true;
}

void createNewEvent()
{
	ev=new eudaq::RawDataEvent(EVENT_TYPE, m_run, m_ev);
}

void setTimeStamp( unsigned long long TimeStamp )
{
	ev->setTimeStamp(TimeStamp);
}

void AddPlane2Event( unsigned plane,const std::vector<unsigned char>& inputVector )
{
	ev->AddBlock(plane, inputVector);
}

void sendEvent()
{
	// Send the event to the Data Collector      
	SendEvent(*ev);

	// clean up 
	delete ev;

	// Now increment the event number

	m_ev++;
}
bool ConfigurationSatus(){
	return isConfigured;
}

	// This is just a dummy class representing the hardware
	// It here basically that the example code will compile
	// but it also generates example raw data to help illustrate the decoder
	clock_t startTime_;
   
	unsigned m_run, m_ev, m_exampleparam;
	bool stopping, done,started,m_eventPending,isConfigured;
	eudaq::eudaqThread SCT_thread;
	eudaq::RawDataEvent *ev;
	SCTProducer* m_interface;
	eudaq::Configuration  m_config;
};

 void * workerthread_thread(void * arg){
	 SCTProducer* sct=(SCTProducer*)arg;
	 sct->send_start_run();
	 return 0;
 }

 




	// The constructor must call the eudaq::Producer constructor with the name
	// and the runcontrol connection string, and initialize any member variables.
	SCTProducer::SCTProducer(const char* name,const char* runcontrol):m_prod(nullptr) {
	//		std::cout<< "hallo from sct producer"<<std::endl;
			
		Connect2RunControl(name,runcontrol);
	}

	SCTProducer::SCTProducer():m_prod(nullptr)
	{

	}

	SCTProducer::~SCTProducer()
	{
		delete m_prod;
	}










	void SCTProducer::Connect2RunControl( const char* name,const char* runcontrol )
	{
		std::string n="tcp://"+std::string(runcontrol);
			m_prod=new Producer_PImpl(name,n,this);
	}

	void SCTProducer::createNewEvent()
	{
		m_prod->createNewEvent();
	}

	void SCTProducer::setTimeStamp( unsigned long long TimeStamp )
	{
		m_prod->setTimeStamp(TimeStamp);
	}

	void SCTProducer::AddPlane2Event( unsigned plane,const std::vector<unsigned char>& inputVector )
	{
		m_prod->AddPlane2Event(plane, inputVector);
	}

	bool SCTProducer::getStarted()
	{
		return m_prod->started;
	}


	void SCTProducer::sendEvent()
	{
		m_prod->sendEvent();
	}

	void SCTProducer::send_onConfigure()
	{
		Emit("send_onConfigure()");
	}

	void SCTProducer::send_onStop()
	{
		Emit("send_onStop()");
	}

	void SCTProducer::send_start_run()
	{
		Emit("send_start_run()");
	}

	bool SCTProducer::getDone()
	{
		return m_prod->done;
	}

	void SCTProducer::setStopped( bool newStat )
	{
		m_prod->stopping=newStat;
	}

	bool SCTProducer::getstopping()
	{
		return m_prod->stopping;
	}

	bool SCTProducer::getConnectionStatus()
	{
		return !(m_prod==nullptr);
	}

	int SCTProducer::getConfiguration( const char* tag, int DefaultValue )
	{
		return m_prod->getConfiguration().Get(tag,DefaultValue);

	}



	int SCTProducer::getConfiguration( const char* tag, const char* defaultValue,char* returnBuffer,Int_t sizeOfReturnBuffer )
	{
		std::string dummy(tag);
		std::string ret= m_prod->getConfiguration().Get(dummy,defaultValue );
		
		if (sizeOfReturnBuffer<ret.size()+1)
		{
			return 0;
		}

		
		strncpy(returnBuffer, ret.c_str(), ret.size());
		returnBuffer[ret.size()]=0;
		return ret.size();
	}

// 	TString SCTProducer::getConfiguration_TString( const char* tag, const char* defaultValue )
// 	{
// 		TString ReturnValue(m_prod->getConfiguration().Get(tag,defaultValue));
// 		std::cout<<ReturnValue.Data()<<std::endl;
// 		return ReturnValue;
// 	}

	bool SCTProducer::ConfigurationSatus()
	{
		return	m_prod->ConfigurationSatus();
	}

// The main function that will create a Producer instance and run it
bool StartTestbeamProducer(const char* nameIn,const char* IP_AdresseIn) {
std::string name(nameIn);//"SCTProducer";
std::string rctrl(IP_AdresseIn);//"tcp://127.0.0.1:44000";

	try {
		// This will look through the command-line arguments and set the options
		
		// Set the Log level for displaying messages based on command-line
//		EUDAQ_LOG_LEVEL(level.Value());
		// Create a producer
		SCTProducer producer(name.c_str(), rctrl.c_str());
		// And set it running...
	//	producer.ReadoutLoop();
		// When the readout loop terminates, it is time to go
		std::cout << "Quitting" << std::endl;
	} catch (...) {
		// This does some basic error handling of common exceptions
		std::cout<<"an error had occurred"<<std::endl;
		return false;
	}
	return true;
}

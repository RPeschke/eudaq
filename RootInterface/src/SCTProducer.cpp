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

 void * workerthread_thread(void * arg){
	 SCTProducer* sct=(SCTProducer*)arg;
	 sct->send_start_run();
	 return 0;
 }
// A name to identify the raw data format of the events generated
// Modify this to something appropriate for your producer.
 




	// The constructor must call the eudaq::Producer constructor with the name
	// and the runcontrol connection string, and initialize any member variables.
	SCTProducer::SCTProducer(const std::string & name, const std::string & runcontrol)
		: eudaq::Producer(name, runcontrol),
		m_run(0), m_ev(0), stopping(false), done(false),started(false),SCT_thread(),m_eventPending(0) {
			std::cout<< "hallo from sct producer"<<std::endl;
	}

	// This gets called whenever the DAQ is configured
void SCTProducer::OnConfigure(const eudaq::Configuration & config) {
		std::cout << "Configuring: " << config.Name() << std::endl;
		send_onConfigure();
		// Do any configuration of the hardware here
		// Configuration file values are accessible as config.Get(name, default)
		m_exampleparam = config.Get("Parameter", 0);
		std::cout << "Example Parameter = " << m_exampleparam << std::endl;
	//	hardware.Setup(m_exampleparam);

		// At the end, set the status that will be displayed in the Run Control.
		SetStatus(eudaq::Status::LVL_OK, "Configured (" + config.Name() + ")");
	}

	// This gets called whenever a new run is started
	// It receives the new run number as a parameter
	void SCTProducer::OnStartRun(unsigned param) {
		// version 0.1 Susanne from LatencyScan.cpp
		std::cout<<"virtual void OnStartRun(unsigned param)"<<std::endl;
		m_run =param;
		m_ev=0;
	//	send2TST_setEventNo(0);
		started=true;
	//	send2TST_setTelStatus(1);
		
		//e->tel_status=1;
		startTime_=clock();
	
	SCT_thread.start(workerthread_thread,this);

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
	void SCTProducer::OnStopRun() {
		std::cerr << "Stopping Run " << std::endl;
		
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
	void SCTProducer::OnTerminate() {
		std::cout << "Terminating..." << std::endl;
		done = true;
	}


	void SCTProducer::createNewEvent()
	{
		ev=new eudaq::RawDataEvent(EVENT_TYPE, m_run, m_ev);
	}

	void SCTProducer::setTimeStamp( unsigned long long TimeStamp )
	{
		ev->setTimeStamp(TimeStamp);
	}

	void SCTProducer::AddPlane2Event( unsigned plane,std::vector<unsigned char>& inputVector )
	{
					ev->AddBlock(plane, inputVector);
	}

	void SCTProducer::sendEvent()
	{
		// Send the event to the Data Collector      
		SendEvent(*ev);
		
		// clean up 
		delete ev;

		// Now increment the event number
		
		m_ev++;
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


// The main function that will create a Producer instance and run it
bool StartTestbeamProducer(const char* nameIn,const char* IP_AdresseIn) {
std::string name(nameIn);//"SCTProducer";
std::string rctrl(IP_AdresseIn);//"tcp://127.0.0.1:44000";

	try {
		// This will look through the command-line arguments and set the options
		
		// Set the Log level for displaying messages based on command-line
//		EUDAQ_LOG_LEVEL(level.Value());
		// Create a producer
		SCTProducer producer(name, rctrl);
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

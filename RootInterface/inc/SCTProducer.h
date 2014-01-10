#ifndef SCT_PRODUCER_H__
#define SCT_PRODUCER_H__

#ifndef __CINT__
#include "eudaq/Event.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/Producer.hh"
#endif

#include "RQ_OBJECT.h"
#include "RTypes.h"
#include <string>




#ifndef __CINT__
#include "../../main/include/eudaq/RawDataEvent.hh"ö
static const std::string EVENT_TYPE = "SCTupgrade";

#endif
// Declare a new class that inherits from eudaq::Producer
class SCTProducer
#ifndef __CINT__
	: public eudaq::Producer 
#endif
	{
	RQ_OBJECT("SCTProducer")
public:


	// The constructor must call the eudaq::Producer constructor with the name
	// and the runcontrol connection string, and initialize any member variables.
	SCTProducer(const std::string & name="nix", const std::string & runcontrol="asd");
#ifndef __CINT__
	// This gets called whenever the DAQ is configured
	virtual void OnConfigure(const eudaq::Configuration & config) ;
#endif
	// This gets called whenever a new run is started
	// It receives the new run number as a parameter
	virtual void OnStartRun(unsigned param);
	// This gets called whenever a run is stopped
	virtual void OnStopRun() ;

	// This gets called when the Run Control is terminating,
	// we should also exit.
	virtual void OnTerminate() ;

	// This is just an example, adapt it to your hardware


	
	// Interface Functions
	bool getstopping(){return stopping;}
	void setStopped(bool newStat){ stopping=newStat;}
	
	
	bool getStarted(){return started;}
	bool getDone(){return done;}
	void createNewEvent();
	void setTimeStamp(unsigned long long TimeStamp);
	void AddPlane2Event(unsigned plane,std::vector<unsigned char>& inputVector);
	void sendEvent();

	// signals
	void send_start_run();
	void send_onConfigure();
	void send_onStop();
#ifndef __CINT__
	std::vector<unsigned char> outputVector;
private:
	// This is just a dummy class representing the hardware
	// It here basically that the example code will compile
	// but it also generates example raw data to help illustrate the decoder
	clock_t startTime_;
	
	unsigned m_run, m_ev, m_exampleparam;
	bool stopping, done,started,m_eventPending;
	eudaq::eudaqThread SCT_thread;
	eudaq::RawDataEvent *ev;
#endif
	ClassDef(SCTProducer,1)
};
#ifndef __CINT__
// The main function that will create a Producer instance and run it
bool StartTestbeamProducer(const char* name,const char* IP_Adresse);
#endif
#ifdef __CINT__
#pragma link C++ class SCTProducer;
#endif


#endif // SCT_PRODUCER_H__

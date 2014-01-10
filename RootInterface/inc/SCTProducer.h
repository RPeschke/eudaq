#ifndef SCT_PRODUCER_H__
#define SCT_PRODUCER_H__



#include "RQ_OBJECT.h"
#include "RTypes.h"








class SCTProducer
	{
	RQ_OBJECT("SCTProducer")
public:


	// The constructor must call the eudaq::Producer constructor with the name
	// and the runcontrol connection string, and initialize any member variables.
	SCTProducer(const char* name,const char* runcontrol);
	SCTProducer();
	~SCTProducer();
	// This is just an example, adapt it to your hardware

	void Connect2RunControl(const char* name,const char* runcontrol);
	bool getConnectionStatus();
	// Interface Functions
	bool getstopping();
	void setStopped(bool newStat);
	bool getStarted();
	bool getDone();	



	bool ConfigurationSatus();
	int getConfiguration(const char* tag, int DefaultValue);
	int getConfiguration(const char* tag, const char* defaultValue,char* returnBuffer,Int_t sizeOfReturnBuffer);
	

	
	void createNewEvent();
	void setTimeStamp(unsigned long long TimeStamp);
	void AddPlane2Event(unsigned plane,const std::vector<unsigned char>& inputVector);
	void sendEvent();

	// signals
	void send_start_run();
	void send_onConfigure();
	void send_onStop();



	class Producer_PImpl;
	Producer_PImpl* m_prod;
	ClassDef(SCTProducer,1)
};

#ifdef __CINT__
#pragma link C++ class SCTProducer;
#endif


#endif // SCT_PRODUCER_H__

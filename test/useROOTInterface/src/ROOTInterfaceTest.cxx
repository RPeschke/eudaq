#include "TSystem.h"
#include "RQ_OBJECT.h"
#include "Rtypes.h"
#include "TSignals.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
class TSignals;
class ROOTProducer;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// SCT DUMMY /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class SCTDummy{
public:
  SCTDummy();
  ~SCTDummy();
  void openfile(const char* name);
  void openfile(std::string& name);

  void setReadoutSpeed(int speed);
  int getTimestamp();

  bool  readLine();

  bool hasData();
  void readOutLoop();
  static const size_t size = 256;
  bool data[size];

  TSignals *eSignals;

private:
  std::ifstream in;
  int _readoutSpeed = 1;

  std::vector < std::string > m_data_vec;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// runSCTClass  //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class runSCTClass{
  RQ_OBJECT("runSCTClass")
public:
  runSCTClass(const char* name);


  void onStart();
  void onConfigure();
  void onStop();
  void OnTerminate();



  void readoutloop();






  enum status_enum{
    unconfigured,
    configured,
    starting,
    started,
    stopping,
    doTerminat

  } m_status;
  ROOTProducer *p;
  SCTDummy *e;
  ClassDef(runSCTClass, 1);
};
void ROOTInterfaceTest(const char* name){
  gSystem->Load("ROOTProducer.dll");
  runSCTClass rSCT(name);

  rSCT.readoutloop();


}

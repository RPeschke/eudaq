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




runSCTClass::runSCTClass(const char* name)
{
  p = new ROOTProducer();
  do
  {
    gSystem->Sleep(20);
    p->Connect2RunControl(name, "127.0.0.1");

  } while (!p->getConnectionStatus());
  e = new SCTDummy();
  m_status = status_enum::unconfigured;
  p->addDataPointer(0, e->data, e->size);

  e->eSignals->Connect("StartOfBurst()", "ROOTProducer", p, "createNewEvent()");
  e->eSignals->Connect("EndOfBurst()", "ROOTProducer", p, "sendEvent()");
  p->Connect("send_onStart()", "runSCTClass", this, "onStart()");
  p->Connect("send_onConfigure()", "runSCTClass", this, "onConfigure()");
  p->Connect("send_onStop()", "runSCTClass", this, "onStop()");
  p->Connect("send_OnTerminate()", "runSCTClass", this, "OnTerminate()");
}

void runSCTClass::onStart()
{
  std::cout << "runSCTClass::onStart()" << std::endl;

  m_status = started;
}

void runSCTClass::onConfigure()
{
  std::cout << "runSCTClass::onConfigure()" << std::endl;
  const int sizeofDummy = 256;
  char dummy[sizeofDummy];
  p->getConfiguration("infile", "c:/slac/EventData_288_75.dat", &dummy[0], sizeofDummy);
  e->openfile(dummy);
  m_status = configured;
}

void runSCTClass::onStop()
{
  std::cout << "runSCTClass::onStop()" << std::endl;

  m_status = stopping;
}

void runSCTClass::OnTerminate()
{
  std::cout << "runSCTClass::OnTerminate()" << std::endl;
  m_status = doTerminat;
}

void runSCTClass::readoutloop()
{
  while (m_status != doTerminat)
  {
    if (m_status != started)
    {
      gSystem->Sleep(20);
      p->checkStatus();
      continue;
    }



    e->readOutLoop();

  }
}







bool SCTDummy::hasData()
{
  gSystem->Sleep(_readoutSpeed);
  return !m_data_vec.empty();
}

bool SCTDummy::readLine()
{
  std::string line = m_data_vec.back();

  size_t j = 0;
  for (int i = 0; i < line.size(); ++i)
  {

    if (j>=size)
    {
      continue;
    }
    if (line.at(i) == '1')
    {
      data[j] = true;
    }
    else
    {
      data[j] = false;
    }
    ++j;
  }

  m_data_vec.pop_back();
  m_data_vec.pop_back();


  return true;
}

void SCTDummy::setReadoutSpeed(int speed)
{
  _readoutSpeed = speed;
}

void SCTDummy::openfile(const char* name)
{

  std::cout << "opening file: " << name << std::endl;
  in.open(name);

  std::string line;
  while (!in.eof())
  {
    std::getline(in, line);
    if (line.empty())
    {
      continue;
    }
    m_data_vec.push_back(line);
  }
  in.close();
  std::cout << "size of file: " << m_data_vec.size() << std::endl;

}

void SCTDummy::openfile(std::string& name)
{
  openfile(name.c_str());
}

SCTDummy::~SCTDummy()
{
  delete eSignals;

  if (in)
  {
    in.close();
  }
}

int SCTDummy::getTimestamp()
{
  return 0;
}

void SCTDummy::readOutLoop()
{
  int i = 0;
  while (!m_data_vec.empty())
  {
    eSignals->StartOfBurst();
    readLine();

    std::cout << i << std::endl;
    eSignals->EndOfBurst();
    ++i;
  }
}

SCTDummy::SCTDummy()
{
  eSignals = new TSignals();
  for (int i = 0; i < size; ++i)
  {
    data[i] = 0;
  }
}

void ROOTInterfaceTest(const char* name){
  gSystem->Load("ROOTProducer.dll");
  runSCTClass rSCT(name);

  rSCT.readoutloop();


}

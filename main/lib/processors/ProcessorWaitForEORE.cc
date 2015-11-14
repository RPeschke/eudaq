#include "eudaq/ProcessorBase.hh"
#include "eudaq/Processors.hh"
#include <atomic>


namespace eudaq {
class addEORE {
public:
  addEORE(bool isEore, std::atomic<int>* eore_counter):m_isEore(isEore),m_EORE_counter(eore_counter) {
  }
  ~addEORE() {
    if (m_isEore)
    {
      ++(*m_EORE_counter);
    }
  }

  bool m_isEore;
  std::atomic<int>* m_EORE_counter;
};

class Processor_WaitForEORE :public ProcessorBase {

public:
  Processor_WaitForEORE(int timeWaiting);
  virtual void init() override;
  virtual void end() override {}
  virtual ReturnParam ProcessEvent(event_sp , ConnectionName_ref con) override;
  virtual void wait() override;
private:
  std::atomic<int> m_numOfBoreEvents = 0, m_numOfEoreEvents = 0;
  int m_timeWaiting;
  std::atomic<int> m_lastEvent;
};

Processor_WaitForEORE::Processor_WaitForEORE(int timeWaiting) : m_timeWaiting(timeWaiting) {
  m_lastEvent = MAXINT32;
}

void Processor_WaitForEORE::init() {
  m_numOfBoreEvents = 0;
  m_numOfEoreEvents = 0;
  m_lastEvent = MAXINT32;

}

ProcessorBase::ReturnParam Processor_WaitForEORE::ProcessEvent(event_sp ev, ConnectionName_ref con) {
  m_lastEvent = static_cast<int>(std::clock());
  if (ev->IsBORE()) {
    ++m_numOfBoreEvents;
  }

  addEORE eore_counter(ev->IsEORE(), &m_numOfEoreEvents);
  return  processNext(std::move(ev), con);
}

void Processor_WaitForEORE::wait() {

 
  while (m_numOfBoreEvents != m_numOfEoreEvents || m_numOfBoreEvents==0) {
    if ((static_cast<int>(std::clock()) - m_lastEvent) > m_timeWaiting) {
      std::cout << "timeout" << std::endl;
      break;
    }
    eudaq::mSleep(100);
  }
}
Processors::processor_up Processors::waitForEORE(int timeIn_ms/*=200*/) {
  return make_Processor_up<Processor_WaitForEORE>(timeIn_ms);
}
}
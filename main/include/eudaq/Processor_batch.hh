#ifndef Processor_batch_h__
#define Processor_batch_h__


#include "eudaq/ProcessorBase.hh"
#include "eudaq/Platform.hh"
#include <memory>
namespace eudaq {

class DLLEXPORT Processor_batch :public ProcessorBase {

public:
  virtual ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con) override;
  Processor_batch(Parameter_ref name);
  virtual ~Processor_batch() {}
  void init() override;
  void end() override;
  void pushProcessor(Processor_up processor);
  void run();
  template <class T, class... Args>
  void pushNewProcessor(Args&&... args) {
    pushProcessor(make_Processor<T>(args...));
  }
private:
  std::unique_ptr<std::vector<Processor_up>> m_processors;

  Processor_rp m_last = nullptr ,m_first =nullptr;
};
}
#endif // Processor_batch_h__
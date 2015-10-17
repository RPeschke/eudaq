#ifndef Processor_batch_h__
#define Processor_batch_h__


#include "eudaq/Processor.hh"
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

private:
  std::unique_ptr<std::vector<Processor_up>> m_processors;

  Processor_rp m_last = nullptr;
};
}
#endif // Processor_batch_h__
#ifndef Processor_batch_h__
#define Processor_batch_h__


#include "eudaq/ProcessorBase.hh"
#include "eudaq/Platform.hh"
#include <memory>
#include "Processor_inspector.hh"
namespace eudaq {
using processor_i_up = std::unique_ptr<Processor_Inspector>;
class DLLEXPORT Processor_batch :public ProcessorBase {

public:
  virtual ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con) override;
  Processor_batch(Parameter_ref name);
  Processor_batch();
  virtual ~Processor_batch();
  void init() override;
  void end() override;
  void pushProcessor(Processor_up processor);
  void pushProcessor(Processor_rp processor);
  void wait() override;
  void run();
  void reset();
  template <class T, class... Args>
  void pushNewProcessor(Args&&... args) {
    pushProcessor(make_Processor_up<T>(args...));
  }
private:
  std::unique_ptr<std::vector<Processor_up>> m_processors;
  std::vector<Processor_rp> m_processors_rp;
  Processor_rp m_last = nullptr ,m_first =nullptr;
};
DLLEXPORT Processor_batch& operator>> (Processor_batch& batch, Processor_up proc);
DLLEXPORT Processor_batch& operator>> (Processor_batch& batch, processor_i_up proc);
DLLEXPORT Processor_batch& operator>> (Processor_batch& batch, Processor_rp proc);

DLLEXPORT std::unique_ptr<Processor_batch> make_batch();
template <typename T>
Processor_batch& operator>> (Processor_batch& batch, T&& lamdbaProcessor) {
  return batch >> make_Processor(std::forward<T>(lamdbaProcessor));
}
template <typename T>
Processor_batch& operator>> (Processor_batch& batch, T* Processor) {
  return batch >> dynamic_cast<Processor_rp> (Processor);
}

}
#endif // Processor_batch_h__
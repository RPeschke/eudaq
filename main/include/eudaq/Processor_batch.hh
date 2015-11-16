#ifndef Processor_batch_h__
#define Processor_batch_h__


#include "eudaq/ProcessorBase.hh"
#include "eudaq/Platform.hh"
#include <memory>
#include "Processor_inspector.hh"
namespace eudaq {
using processor_i_up = std::unique_ptr<Processor_Inspector>;
using processor_i_rp = Processor_Inspector*;
class DLLEXPORT Processor_batch :public ProcessorBase {

public:
  virtual ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con) override;
  Processor_batch();
  virtual ~Processor_batch();
  void init() override;
  void end() override;
  void pushProcessor(Processor_up processor);
  void pushProcessor(Processor_rp processor);
  void wait() override;
  void run();
  void reset();
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


class Processor_i_batch :public Processor_Inspector {
public:
  virtual ReturnParam inspectEvent(const Event& ev, ConnectionName_ref con) override;
  Processor_i_batch();
  void init() override;
  void end() override;
  void wait() override;
  void pushProcessor(processor_i_up processor);
  void pushProcessor(processor_i_rp processor);

  void reset();
private:
  std::unique_ptr<std::vector<processor_i_up>> m_processors;
  std::vector<processor_i_rp> m_processors_rp;
  Processor_rp m_last = nullptr, m_first = nullptr;
};
}
#endif // Processor_batch_h__
#include "eudaq/Processor_batch.hh"
#include <memory>


namespace eudaq {
using ReturnParam = ProcessorBase::ReturnParam;
Processor_batch::Processor_batch(Parameter_ref name) :ProcessorBase(name), m_processors(__make_unique<std::vector<Processor_up>>()) {


}



ReturnParam Processor_batch::ProcessEvent(event_sp ev, ConnectionName_ref con) {
  return m_first->ProcessEvent(ev, con);
}

void Processor_batch::init() {

  for (auto& e : *m_processors) {
    e->init();
  }
  m_first = m_processors->front().get();
}

void Processor_batch::end() {
  for (auto& e : *m_processors) {
    e->end();
  }
}

void Processor_batch::pushProcessor(Processor_up processor) {

  m_processors->push_back(std::move(processor));
  auto new_last = m_processors->back().get();
  if (m_last) {
    m_last->AddNextProcessor(new_last);
  }

  m_last = new_last;

}



void Processor_batch::run() {
  ReturnParam ret = sucess;
  do {
    ret = ProcessEvent(nullptr, 0);
    if (ret == ret_error) {
      std::cout << "an error occurred " << std::endl;
    }
  } while (ret != stop);
} 

}
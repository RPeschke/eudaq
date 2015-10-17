#include "eudaq/Processor_batch.hh"
#include <memory>


namespace eudaq {
  using ReturnParam = ProcessorBase::ReturnParam;
  Processor_batch::Processor_batch(Parameter_ref name) :ProcessorBase(name), m_processors(__make_unique<std::vector<Processor_up>>())
  {
    

  }



  ReturnParam Processor_batch::ProcessEvent(event_sp ev, ConnectionName_ref con) {
    ReturnParam ret = sucess;
    if (ev) {
      std::cout << "input not supported " << std::endl;
      ev = nullptr;
    }
    do {
      ret = m_processors->front()->ProcessEvent(ev,con);

      if (ret == ret_error) {
        std::cout << "an error occurred " << std::endl;

      }
    } while (ret != stop);
    return ret;
  }

  void Processor_batch::init() {
        
    for (auto& e : *m_processors) {
      e->init();
    }
  }

  void Processor_batch::end() {
    for (auto& e : *m_processors) {
      e->init();
    }
  }

  void Processor_batch::pushProcessor(Processor_up processor)
  {
    
    m_processors->push_back(std::move(processor));
    auto new_last = m_processors->back().get();
    if (m_last)
    {
      m_last->AddNextProcessor(new_last);
    }
    
    m_last = new_last;

  }



}
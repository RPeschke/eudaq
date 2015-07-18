#include "eudaq/Processor_batch.hh"
#include <memory>


namespace eudaq {
  using ReturnParam = ProcessorBase::ReturnParam;
  Processor_batch::Processor_batch(Parameter_ref name) :Processor(name)
  {
    
//     auto splitted_names = eudaq::split(name,",");
//     for (auto& e:splitted_names)
//     {
// 
//       pushProducer(ProcessorFactory::create(e, ""));
//     }

  }

  void Processor_batch::initialize()
  {
    for (auto& e:m_processors)
    {
      e->init();
    }
  }

  void Processor_batch::Finish()
  {

    for (auto& e : m_processors)
    {
      e->end();
    }
  }

  ProcessorBase* Processor_batch::getProcessor(const std::string& name /*= ""*/)
  {
    return this;
  }


  void Processor_batch::AddProcessor(ProcessorBase* next,const std::string& name/*= ""*/)
  {
    m_next = next;
    m_processors.back()->AddProcessor(next, name);
  }

  void Processor_batch::print(std::ostream& os)
  {
    os << getName();
  }

  ReturnParam Processor_batch::ProcessEvent(event_sp ev)
  {
    ReturnParam ret =sucess;
    if (ev)
    {
      std::cout << "input not supported " << std::endl;
      ev = nullptr;
    }
    do 
    {
      ret = m_processors.front()->ProcessEvent(ev);

      if (ret == ret_error){
        std::cout << "an error occurred " << std::endl;
        
      }
    } while (ret!= stop );
    return ret;
  }

  void Processor_batch::pushProducer(std::unique_ptr<ProcessorBase> processor)
  {
    if (processor&&!m_processors.empty())
    {

      AddProcessor(processor.get(),processor->getName());
    }
    m_processors.push_back(std::move(processor));
    m_next = m_processors.back().get();
  }



  RegisterProcessor(Processor_batch, ProcessorNames::batch());
}
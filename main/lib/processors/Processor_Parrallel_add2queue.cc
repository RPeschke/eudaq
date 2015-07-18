#include "eudaq/Processor_Parrallel_add2queue.hh"
#include "eudaq/Processor.hh"

namespace eudaq{

  
  using ReturnParam = ProcessorBase::ReturnParam;
  
  class Processor_Parrallel_add2queue::interfaceProducer : public Processor{
  public:
    
    interfaceProducer(Parameter_ref conf):Processor(conf) {}
    virtual void initialize(){

      auto base = dynamic_cast<Processor_Parrallel_add2queue*>(m_base);
      m_next = base->getNextProcessor(getName());
    }

    void setBaseProcessor(Processor_rp base){ m_base = base; }

    virtual ReturnParam ProcessEvent(event_sp ev) {
    
      auto ret = m_base->ProcessEvent(nullptr);
      if (ret!=ReturnParam::sucess)
      {
        return ret;
      }

      if (ev)
      {
        return ProcessNext(std::move(ev));
      }

      return ret;
    };
        
  private:
    
    Processor_rp  m_base = nullptr;

  };

 





  Processor_Parrallel_add2queue::Processor_Parrallel_add2queue(Parameter_ref conf) :Processor_N_2_M_base(conf)
  {

  }

  void Processor_Parrallel_add2queue::initialize()
  {
    for (auto& e:m_inputInterface)
    {
      e.second->init();
    }
    
  }





  Processor_up Processor_Parrallel_add2queue::CreateInterface(ConnectionName_ref name, Parameter_ref conf)
  {
  
      auto proc= Processor_up(new Processor_Parrallel_add2queue::interfaceProducer(name));
    
      
      auto p_raw = dynamic_cast<interfaceProducer*>(proc.get());
      p_raw->setBaseProcessor(this);
   
      return proc;
  }

}
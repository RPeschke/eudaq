#include "eudaq/Processor_Parrallel_add2queue.hh"
#include "eudaq/Processor.hh"

namespace eudaq{

  
  using ReturnParam = ProcessorBase::ReturnParam;
  
  class Processor_Parrallel_add2queue::interfaceProducer : public Processor{
  public:
    
    interfaceProducer(Parameter_ref conf):Processor(conf) {}
    virtual void init(){

      auto base = dynamic_cast<Processor_Parrallel_add2queue*>(m_base);
      m_next = base->getNextProcessor(getName());
    }

    void setBaseProcessor(Processor_rp base){ m_base = base; }

    virtual ReturnParam ProcessorEvent(event_sp ev) {
    
      auto ret = m_base->ProcessorEvent(nullptr);
      if (ret!=ReturnParam::sucess)
      {
        return ret;
      }

      if (ev)
      {
        return ProcessNext(ev);
      }

      return ret;
    };
        
  private:
    
    Processor_rp  m_base = nullptr;

  };

 

  ProcessorBase* Processor_Parrallel_add2queue::getProcessor(ConnectionName_ref name /*= ""*/)
  {
 


    auto inputItt = m_inputInterface.find(name);


    if (inputItt == m_inputInterface.end())
    {
      m_inputInterface[name] = Processor_up(new Processor_Parrallel_add2queue::interfaceProducer(name));
      auto p_raw = dynamic_cast<interfaceProducer*>(m_inputInterface[name].get());
      p_raw->setBaseProcessor(this);
      if (m_nextProcessor)
      {
      m_inputInterface[name]->AddProcessor(getNextProcessor(name), name);
      }

    }

    return m_inputInterface[name].get();
  }

  Processor_rp Processor_Parrallel_add2queue::getNextProcessor(ConnectionName_ref name /*= ""*/)
  {
    if (m_nextProcessor)
    {
      return m_nextProcessor->getProcessor(name);
    }

    return nullptr;
  }

  void Processor_Parrallel_add2queue::AddProcessor(Processor_rp next, ConnectionName_ref name /*= ""*/)
  {
    if (name.empty()){
      m_nextProcessor = next->getProcessor(getName());

    }
    else{
      m_nextProcessor = next->getProcessor(name);
    }
  }

  std::string Processor_Parrallel_add2queue::getName()
  {
    return m_conf;
  }

  Processor_Parrallel_add2queue::Processor_Parrallel_add2queue(Parameter_ref conf) :ProcessorBase(conf)
  {

  }

  void Processor_Parrallel_add2queue::init(Configuration_ref conf)
  {
    for (auto& e:m_inputInterface)
    {
      e.second->init(conf);
    }
    
  }





}
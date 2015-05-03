#include "eudaq/Processor_inspector.hh"




namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class ShowEventNR :public Processor_Inspector{
  public:


    virtual ReturnParam inspecktEvent(const Event&);

    virtual void print(std::ostream& os);


    
    ShowEventNR(Parameter_ref);
    size_t m_pos;
  };
  RegisterProcessor(ShowEventNR, ProcessorNames::show_event_nr());




  void ShowEventNR::print(std::ostream& os)
  {
    os << getName();
  }

  ShowEventNR::ShowEventNR(Parameter_ref conf) :Processor_Inspector(conf)
  {
    m_pos =4* ProConfig::getProcessorParallelPos(conf);
  }



  ReturnParam ShowEventNR::inspecktEvent(const Event& ev)
  {
    if (ev.GetEventNumber() <10)
    {
      std::cout << std::string(m_pos, ' ') << getName() << ": " << ev.GetEventNumber() << std::endl;
    }
    else if (ev.GetEventNumber()% 100 ==0)
    {
      std::cout << std::string(m_pos, ' ') << getName() << ": " << ev.GetEventNumber() << std::endl;
    }
   
    return sucess;
  }

}


#include "eudaq/Processor_inspector.hh"




namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class ShowEventNR :public Processor_Inspector{
  public:


    virtual ReturnParam inspecktEvent(const Event&);

    virtual std::string getName() override;
    virtual void print(std::ostream& os);


    
    ShowEventNR(Parameter_ref);
    std::string m_name;
  };
  RegisterProcessor(ShowEventNR, ProcessorNames::show_event_nr());


  std::string ShowEventNR::getName()
  {
    return m_name;
  }

  void ShowEventNR::print(std::ostream& os)
  {
    os << getName();
  }

  ShowEventNR::ShowEventNR(Parameter_ref conf) :Processor_Inspector(conf)
  {
    m_name = conf;
  }



  ReturnParam ShowEventNR::inspecktEvent(const Event& ev)
  {
    std::cout << getName() << ": "<<ev.GetEventNumber() << std::endl;
    return sucess;
  }

}


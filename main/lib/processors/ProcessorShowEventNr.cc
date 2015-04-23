#include "eudaq/Processor_inspector.hh"




namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class ShowEventNR :public Processor_Inspector{
  public:


    virtual ReturnParam inspecktEvent(const Event&);

    virtual std::string getName() override;
    virtual void print(std::ostream& os);


    
    ShowEventNR(Parameter_ref);
  
  };
  RegisterProcessor(ShowEventNR, "ShowEventNR");


  std::string ShowEventNR::getName()
  {
    return "show event nr";
  }

  void ShowEventNR::print(std::ostream& os)
  {
    os << getName();
  }

  ShowEventNR::ShowEventNR(Parameter_ref conf) :Processor_Inspector(conf)
  {

  }



  ReturnParam ShowEventNR::inspecktEvent(const Event& ev)
  {
    std::cout << ev.GetEventNumber() << std::endl;
    return sucess;
  }

}


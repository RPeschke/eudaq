#include "eudaq/Processor.hh"




namespace eudaq{
  class ShowEventNR :public Processor{
  public:


    virtual void ProcessorEvent(event_sp ev) ;

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

  ShowEventNR::ShowEventNR(Parameter_ref conf) :Processor(conf)
  {

  }

  void ShowEventNR::ProcessorEvent(event_sp ev)
  {
    std::cout << ev->GetEventNumber() << std::endl;
    ProcessNext(ev);
  }

}


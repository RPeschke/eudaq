#include "eudaq/Processor_inspector.hh"
#include "eudaq/OptionParser.hh"



namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class Process_eventOfInterest :public Processor_Inspector{
  public:
    

    virtual ReturnParam inspecktEvent(const Event&);

  
    virtual void print(std::ostream& os);


    
    Process_eventOfInterest(Parameter_ref);
    
  private:
    std::vector <unsigned> m_events;
    bool CheckIfCurrentEventIsBeyondLastElementOfIntrest(unsigned eventNR);
    bool CheckIfElementIsElementOfIntrest(unsigned eventNR);
  };
  RegisterProcessor(Process_eventOfInterest, ProcessorNames::events_of_intresst());



  void Process_eventOfInterest::print(std::ostream& os)
  {
    os << getName();
  }

  Process_eventOfInterest::Process_eventOfInterest(Parameter_ref conf) :Processor_Inspector("EventsOfIntrest")
  {
    m_events = parsenumbers(ProConfig::getTag(conf,"base","events",""));
  }



  ReturnParam Process_eventOfInterest::inspecktEvent(const Event& ev)
  {
    if (ev.IsBORE()||ev.IsEORE())
    {
      return sucess;
    }
    if (m_events.empty())
    {
      return sucess;
    }

    if (CheckIfCurrentEventIsBeyondLastElementOfIntrest(ev.GetEventNumber()))
    {
      return stop;
    }
    if (!CheckIfElementIsElementOfIntrest(ev.GetEventNumber()))
    {
      return skip_event;
    }
    return sucess;
  }


  bool Process_eventOfInterest::CheckIfCurrentEventIsBeyondLastElementOfIntrest(unsigned eventNR){
    if (eventNR> m_events.back())
    {
      return true;
    }
    return false;
  }
  bool Process_eventOfInterest::CheckIfElementIsElementOfIntrest(unsigned eventNR){

    if (std::find(m_events.begin(), m_events.end(),eventNR) != m_events.end())
    {
      return true;
    }
    return false;
  }
}


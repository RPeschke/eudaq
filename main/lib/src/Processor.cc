#include "eudaq/Processor.hh"



namespace eudaq{

  


  void Processor::init()
  {
    std::cout << " init " << std::endl;
  }



  ProcessorBase* Processor::getProcessor(const std::string& name /*= ""*/)
  {
    m_connection = name;
    return this;
  }



  void Processor::AddProcessor(ProcessorBase* next, const std::string& name)
  {
    if (name.empty()){
      m_next = next->getProcessor(getName());
    
    }
    else{
      m_next = next->getProcessor(name);
    }
  }

  std::string Processor::getName()
  {
    return "some name";
  }

  void Processor::print(std::ostream& os)
  {
    os << getName();
  }

  Processor::Processor(Parameter_ref name) :ProcessorBase(name)
  {

  }

  void Processor::end()
  {
    std::cout << "end" << std::endl;
  }

  void Processor::ProcessNext(event_sp ev)
  {
    if (m_next)
    {
    m_next->ProcessorEvent(ev);
    }
  }

}
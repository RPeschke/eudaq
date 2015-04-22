#include "eudaq/Processor.hh"



namespace eudaq{

  RegisterProcessor(Processor, "processor");


  void Processor::init()
  {
    std::cout << " init " << std::endl;
  }

  void Processor::ProcessorEvent(event_sp ev)
  {
    std::cout << "void Processor::ProcessorEvent(event_sp ev)  "<< ev->GetEventNumber() << std::endl;
    m_next->ProcessorEvent(std::move(ev));
  }


  ProcessorBase* Processor::getProcessor(std::string name /*= ""*/)
  {
    return this;
  }

  void Processor::AddProcessor(ProcessorBase* next, std::string /*= ""*/)
  {
    m_next = next;
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

}
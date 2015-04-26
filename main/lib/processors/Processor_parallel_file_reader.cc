#include "eudaq/Processor_parallel_file_reader.hh"
#include "eudaq/ProcessorBase.hh"


namespace eudaq{


  using ReturnParam = ProcessorBase::ReturnParam;

  RegisterProcessor(Processor_parallel_file, ProcessorNames::Parallel_file_reader());
  void Processor_parallel_file::init()
  {
    m_reader = ProcessorFactory::create(ProcessorNames::file_reader(), m_conf);
    m_reader->init();
    m_reader->AddProcessor(getNextProcessor(getName()), getName());
    Processor_Parrallel_add2queue::init();
  }

  void Processor_parallel_file::end()
  {
    m_reader->end();
  }

  ReturnParam Processor_parallel_file::ProcessorEvent(event_sp ev)
  {
    return m_reader->ProcessorEvent(ev);
  }

  void Processor_parallel_file::print(std::ostream& os)
  {
    os << getName();
  }

  Processor_parallel_file::Processor_parallel_file(Parameter_ref conf) :Processor_Parrallel_add2queue(conf)
  {

  }

}
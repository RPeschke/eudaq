#include "eudaq/Processor_parralel.hh"


namespace eudaq{

  RegisterProcessor(Processor_parallel, ProcessorNames::Parallel_processor());
  void Processor_parallel::finish()
  {

  }

  void Processor_parallel::initialize(Configuration_ref conf)
  {
    m_type = ProConfig::getTag(conf, getName(), "type", "");

    

  }

  Processor_up Processor_parallel::CreateInterface(ConnectionName_ref name, Parameter_ref conf)
  {
    auto type = ProConfig::getProcessorParallelType(m_conf);
    return ProcessorFactory::create(type, ProConfig::ProcessorName(name));
  }

  std::string Processor_parallel::getName()
  {
    return std::string("multi")+ProConfig::getProcessorName(m_conf);
  }

  void Processor_parallel::print(std::ostream& os)
  {
    os << getName();
  }



}
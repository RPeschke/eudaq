#include "eudaq/ProcessorBase.hh"
#include "eudaq/factoryDef.hh"
#include "eudaq/OptionParser.hh"

namespace eudaq{
  
  using Name_t = ProcessorNames::Name_t;
  Name_t ProcessorNames::batch()
  {
    return "Processor_batch";
  }

  Name_t ProcessorNames::buffer()
  {
    return "buffer";
  }

  Name_t ProcessorNames::busy_test()
  {
    return "busy_test";
  }

  Name_t ProcessorNames::multi_buffer()
  {
    return "multi_buffer";
  }

  Name_t ProcessorNames::file_reader()
  {
    return "file_reader";
  }

  Name_t ProcessorNames::file_writer()
  {
    return "file_writer";
  }

  Name_t ProcessorNames::show_event_nr()
  {
    return "show_event_nr";
  }

  Name_t ProcessorNames::events_of_intresst()
  {
    return "event_of_intressted";
  }

  Name_t ProcessorNames::splitter()
  {
    return "splitter";
  }

  Name_t ProcessorNames::merger()
  {
    return "merger";
  }

  registerBaseClassDef(ProcessorBase);


  ProcessorBase::ProcessorBase(Parameter_ref name) :m_conf(name)
  {

  }
  std::unique_ptr<ProcessorBase> ProcessorFactory::create(ProcessorBase::MainType type, ProcessorBase::Parameter_ref param)
  {

    return EUDAQ_Utilities::Factory<ProcessorBase>::Create(type, param);
  }

  void ProcessorFactory::addComandLineOptions(eudaq::OptionParser & op)
  {

  }

  std::string ProcessorFactory::Help_text()
  {
    return "help";
  }

  ProcessorBase::Parameter_t ProcessorFactory::getDefault()
  {
    return "default";
  }

  class ProcessorFactory::Impl{

  public:
    ProcessorBase::Parameter_t m_default_filePattern;

  };

  ProcessorFactory::Impl& ProcessorFactory::getImpl()
  {
    static ProcessorFactory::Impl m_impl;
    return m_impl;
  }

  
}

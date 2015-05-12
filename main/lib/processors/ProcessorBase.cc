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

  Name_t ProcessorNames::Parallel_file_reader()
  {
    return "Parallel_file_reader";
  }

  Name_t ProcessorNames::Parallel_processor()
  {
    return "parallel_processor";
  }


  using Config = ProConfig::Config;

  Name_t ProcessorNames::Print_Configuration()
  {
    return "config";
  }

  Name_t ProcessorNames::SelectEvents()
  {
    return "select_events";
  }

  Config ProConfig::Topic(const std::string& name)
  {
	  return "[" + name + "]\n";
  }

  registerBaseClassDef(ProcessorBase);


  static const std::string ProcessorNameTag = "ProcessorName";
  static const std::string FileNameTag = "FileName";
  static const std::string ProcessorTypeTag = "ProcessorType";
  static const std::string ProcessorBaseTopic = "base";
  static const std::string ProcessorParallelTypeTag = "ParrallelType";
  static const std::string ProcessorParallelPosTag = "ParrallelPos";


  Config ProConfig::Tag(const std::string& tagName, const std::string& tagValue)
  {
	  return tagName + "=" + tagValue + "\n";
  }

  Config ProConfig::ProcessorName(const std::string& Name)
  {
    return Tag(ProcessorNameTag, Name);
  }

  Config ProConfig::ProcessorType(const std::string& Name)
  {
    return Tag(ProcessorTypeTag, Name);
  }

  Config ProConfig::Filename(const std::string& fileName)
  {
    return Tag(FileNameTag, fileName);
  }

  Config ProConfig::ProcessorParallelType(const std::string& Name)
  {
    return Tag(ProcessorParallelTypeTag, Name);
  }

  Config ProConfig::ProcessorParallelPos(size_t pos)
  {
    return Tag(ProcessorParallelPosTag, to_string(pos));
  }

  Config ProConfig::getConfig_configuration(const std::string& name, const std::string& sectionTag)
  {
    return ProConfig::Topic(name) + ProConfig::Tag("section_and_TAG", sectionTag);
  }

  Config ProConfig::getConfig_ConcatSectionsTag(const std::string& section, const std::string& TAGS)
  {
    return section + "%" + TAGS + "&";
  }

  Config ProConfig::eventSelection_configuration(const std::string& name, const std::string& events, bool doBore/*=false*/, bool doEore/*=false*/)
  {
    return ProConfig::Topic(name) + ProConfig::Tag("events", events) + ProConfig::Tag("doBore", to_string(doBore)) + ProConfig::Tag("doEore", to_string(doEore));
  }

  std::string ProConfig::getTag(const ConfigInput& conf, const std::string& section, const std::string& tag, const std::string& def)
  {
    Configuration c(conf);

    c.SetSection(section);
    return c.Get(tag, def);
  }

  std::string ProConfig::getFilename(const ConfigInput& conf, const std::string& section, const std::string& def)
  {
    return getTag(conf, section, FileNameTag, def);
  }

  std::string ProConfig::getProcessorName(const ConfigInput& conf)
  {
    return  getTag(conf, ProcessorBaseTopic, ProcessorNameTag, "");
  }

  std::string ProConfig::getProcessorType(const ConfigInput& conf)
  {
    return getTag(conf, ProcessorBaseTopic, ProcessorTypeTag, "");
  }

  std::string ProConfig::getProcessorParallelType(const ConfigInput& conf)
  {
    return getTag(conf, ProcessorBaseTopic, ProcessorParallelTypeTag, "");
  }

  size_t ProConfig::getProcessorParallelPos(Config conf)
  {
    return std::atol(getTag(conf, ProcessorBaseTopic, ProcessorParallelPosTag, "0").c_str());
  }

  ProcessorBase::ProcessorBase(Parameter_ref name) :m_conf(name)
  {
    m_name = ProConfig::getProcessorName(m_conf);
  }

  std::string ProcessorBase::getName()
  {

    return m_name;
  }

  std::unique_ptr<ProcessorBase> ProcessorFactory::create(ProcessorBase::MainType type, ProcessorBase::Parameter_ref param)
  {

    return EUDAQ_Utilities::Factory<ProcessorBase>::Create(type, ProConfig::Topic(ProcessorBaseTopic) +ProConfig::ProcessorType(type)  + param );
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

  template<>
  ProcessorBase::ConnectionName_t concatenate(ProcessorBase::ConnectionName_ref first, std::string second)
  {
    return first + second;
  }

  
}

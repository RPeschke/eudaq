#include "eudaq\ProcessorBase.hh"
#include "eudaq\OptionParser.hh"
#include "eudaq\FileWriter.hh"
#include "eudaq\Logger.hh"
#include "eudaq\Processor_inspector.hh"
#include "eudaq\baseFileReader.hh"
#include "eudaq\EventSynchronisationBase.hh"

using namespace eudaq;

class getConfig :public Processor_Inspector{
public:
  getConfig(Parameter_ref conf) :Processor_Inspector(conf){
  
  
  }
  virtual void initialize(Configuration_ref conf) {
    m_tag = ProConfig::getTag(conf, getName(), "tag", "");
    m_section = ProConfig::getTag(conf, getName(), "section", "");
  }
  virtual ReturnParam inspecktEvent(const Event&ev) {
  
    if (!ev.IsBORE())
    {
      return ProcessorBase::stop;
    }


    Configuration conf(ev.GetTag("CONFIG", ""),m_section);
    if (m_tag.empty())
    {
      conf.Save(std::cout);
      //conf.Print();
    }
    else{

      std::cout << m_tag << " = " << conf.Get(m_tag, "") << std::endl;
    }

  }
  
  
  std::string m_section, m_tag;
  static  Configuration_t setSection(const std::string& section){
   return ProConfig::Tag("section", section);
  }
  static  Configuration_t setTag(const std::string& tag){
    return ProConfig::Tag("tag", tag);
  }
};
RegisterProcessor(getConfig, "config");
int main(int /*argc*/, const char ** argv) {
 

  std::clock_t    start = std::clock();
  eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);


  FileReaderFactory::addComandLineOptions(op);

  op.ExtraHelpText(FileReaderFactory::Help_text());

  EventSyncFactory::addComandLineOptions(op);

  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
    "The minimum level for displaying log messages locally");

  try {
  op.Parse(argv);
  EUDAQ_LOG_LEVEL(level.Value());



  auto pro = ProcessorFactory::create(ProcessorNames::batch(), "");
  pro->pushProducer(ProcessorFactory::create(ProcessorNames::file_reader(), ProConfig::ProcessorName("first")));
  
  pro->pushProducer(ProcessorFactory::create("config", ProConfig::ProcessorName("getConf")));

  auto conf = ProConfig::Topic("first") + ProConfig::Filename(op.GetArg(0)) + ProConfig::Topic("getConf") + getConfig::setSection("Producer.TLU") + getConfig::setTag("AndMask");
  pro->init(conf);
  pro->ProcessorEvent(nullptr);
  pro->end();
  }
  catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  return 0;
}

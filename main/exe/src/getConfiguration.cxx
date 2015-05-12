#include "eudaq\ProcessorBase.hh"
#include "eudaq\OptionParser.hh"
#include "eudaq\FileWriter.hh"
#include "eudaq\Logger.hh"
#include "eudaq\Processor_inspector.hh"
#include "eudaq\baseFileReader.hh"
#include "eudaq\EventSynchronisationBase.hh"

using namespace eudaq;






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
  
  pro->pushProducer(ProcessorFactory::create(ProcessorNames::Print_Configuration(), ProConfig::ProcessorName("config")));
  pro->pushProducer(ProcessorFactory::create(ProcessorNames::SelectEvents(), ProConfig::ProcessorName("eventOfintresst")));
  pro->pushProducer(ProcessorFactory::create(ProcessorNames::show_event_nr(), "event_nr"));


  auto conf = ProConfig::Topic("first") + ProConfig::Filename(op.GetArg(0)) + ProConfig::getConfig_configuration("config", ProConfig::getConfig_ConcatSectionsTag("Producer.TLU", "AndMask")) +ProConfig::eventSelection_configuration("eventOfintresst","1,2,3",1,1);
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

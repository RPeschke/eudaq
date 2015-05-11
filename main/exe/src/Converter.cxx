#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include "eudaq/MultiFileReader.hh"
#include "eudaq/readAndProcessDataTemplate.h"
#include <iostream>
#include "eudaq/EventSynchronisationDetectorEvents.hh"

#include "eudaq/ProcessorBase.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Processor_inspector.hh"



using namespace eudaq;
unsigned dbg = 0;

class test : public Processor_Inspector{
public:
  test(Parameter_ref conf) :Processor_Inspector(conf){}
  virtual ReturnParam inspecktEvent(const Event&) { std::cout << "hello from test" << std::endl; return ProcessorBase::sucess; };
};
std::string testName(){
  return "test";
}

RegisterProcessor(test, testName());
int main(int, char ** argv) {



    std::clock_t    start = std::clock();
    eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);

    auto events = ReadAndProcess<eudaq::FileWriter>::add_Command_line_option_EventsOfInterest(op);


    FileWriterFactory::addComandLineOptions(op);
    op.ExtraHelpText(FileWriterFactory::Help_text());


    FileReaderFactory::addComandLineOptions(op);

    op.ExtraHelpText(FileReaderFactory::Help_text());

    EventSyncFactory::addComandLineOptions(op);

    eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
                                     "The minimum level for displaying log messages locally");

    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());





  try {



    auto pro = ProcessorFactory::create(ProcessorNames::batch(), "");
    event_sp ev = std::dynamic_pointer_cast<Event>(std::make_shared<eudaq::RawDataEvent>("tesT", 1, 1));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::Parallel_file_reader(), ProConfig::ProcessorName("first")));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::Parallel_file_reader(), ProConfig::ProcessorName("second")));


  //  pro->pushProducer(ProcessorFactory::create("test", ""));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::Parallel_processor(), ProConfig::ProcessorParallelType(ProcessorNames::show_event_nr())));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::splitter(), ""));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::Parallel_processor(), ProConfig::ProcessorParallelType(ProcessorNames::show_event_nr())));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::multi_buffer(), ""));

    pro->pushProducer(ProcessorFactory::create(ProcessorNames::merger(), ""));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::events_of_intresst(), ProConfig::Tag("events", events->Value())));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::show_event_nr(), ProConfig::ProcessorName("buffer")));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::buffer(), ""));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::busy_test(), ""));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::show_event_nr(), "busy"));
    pro->pushProducer(ProcessorFactory::create(ProcessorNames::file_writer(), ""));

    //auto p = (eudaq::Processor_batch*) pro.get();
    //p->AddProcessor2Batch(std::move(ProcessorFactory::create("eventOfInterest", events->Value())));
    //p->AddProcessor2Batch(std::move(ProcessorFactory::create("ProcessorFileWriter", "")));

//     auto conf = ProConfig::Topic("second") + ProConfig::Filename("..\\data\\run000022_.raw") + ProConfig::Topic("first") + ProConfig::Filename("..\\data\\run000022_.raw");;
//       pro->init(conf);
// 
//     pro->ProcessorEvent(ev);
// 
// 
//     pro->end();














    ReadAndProcess<eudaq::FileWriter> readProcess;
    readProcess.setEventsOfInterest(parsenumbers(events->Value()));

    readProcess.addFileReader(FileReaderFactory::create(op));
    readProcess.setWriter(FileWriterFactory::Create());

    readProcess.StartRun();
    readProcess.process();
    readProcess.EndRun();

  }
  catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  std::cout << "Time: " << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  if (dbg > 0)std::cout << "almost done with Converter. exiting" << std::endl;
  return 0;
}

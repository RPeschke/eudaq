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

#include "eudaq/Processor_parallel_file_reader.hh"
#include "eudaq/Processor_batch.hh"
#include "eudaq/Platform.hh"


using namespace eudaq;
unsigned dbg = 0;

class test : public Processor_Inspector{
public:
  test(Parameter_ref conf) :Processor_Inspector(conf){}
  virtual ReturnParam inspecktEvent(const Event&) { std::cout << "hello from test" << std::endl; return ProcessorBase::sucess; };
 static std::unique_ptr<test> create(Parameter_ref conf){ return   std::unique_ptr<test>(new test(conf)); }
};
std::string testName(){
  return "test";
}

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



    auto pro = std::unique_ptr<Processor_batch>( new Processor_batch(ProcessorConf("name")));
    event_sp ev = std::dynamic_pointer_cast<Event>(std::make_shared<eudaq::RawDataEvent>("tesT", 1, 1));

    pro->pushNewProcessor<Processor_parallel_file>(ProcessorConf("first"), fileName("..\\data\\run000022_.raw"));
    pro->pushNewProcessor<Processor_parallel_file>(ProcessorConf("second"), fileName("..\\data\\run000022_.raw"));
    //auto p = (eudaq::Processor_batch*) pro.get();
    //p->AddProcessor2Batch(std::move(ProcessorFactory::create("eventOfInterest", events->Value())));
    //p->AddProcessor2Batch(std::move(ProcessorFactory::create("ProcessorFileWriter", "")));

//     auto conf = ProConfig::Topic("second") + ProConfig::Filename("..\\data\\run000022_.raw") + ProConfig::Topic("first") + ProConfig::Filename("..\\data\\run000022_.raw");;
       pro->init();
// 
     pro->ProcessEvent(ev);
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

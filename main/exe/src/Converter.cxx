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
#include "eudaq/Platform.hh"
#include "eudaq/Processor_batch.hh"
#include "eudaq/ProcessorFileReader.hh"
#include "eudaq/Processor_inspector.hh"
#include "eudaq/ProcessorFileWriter.hh"
#include "eudaq/Processor_eventSelectors.hh"
using namespace eudaq;
unsigned dbg = 0;



int main(int, char ** argv) {

  std::clock_t    mid_ = 0 ,end_=0;

    eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);

    auto events = ReadAndProcess<eudaq::FileWriter>::add_Command_line_option_EventsOfInterest(op);


    FileWriterFactory::addComandLineOptions(op);
    op.ExtraHelpText(FileWriterFactory::Help_text());


    FileReaderFactory::addComandLineOptions(op);

    op.ExtraHelpText(FileReaderFactory::Help_text());

    EventSyncFactory::addComandLineOptions(op);

    eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
                                     "The minimum level for displaying log messages locally");

    std::clock_t    start = std::clock();
  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());

    Processor_batch batch(ProcessorBase::Parameter_t(""));

    batch.pushNewProcessor<ProcessorFileReader>(op);
    batch.pushProcessor(make_Processor([](const Event& ev, ProcessorBase::ConnectionName_ref con) {
      if (ev.GetEventNumber()%1000==0)
      {
        std::cout << ev.GetEventNumber() << "  " <<con << std::endl;
      }
      return ProcessorBase::sucess;
     }));
    batch.pushNewProcessor<select_events>(parsenumbers(events->Value()));
    batch.pushNewProcessor<ProcessorFileWriter>();

    batch.init();
    batch.run();
    batch.end();
    
  }
  catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  end_ = std::clock();
  std::cout << "Time: " << (end_ - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  if (dbg > 0)std::cout << "almost done with Converter. exiting" << std::endl;
  return 0;
}

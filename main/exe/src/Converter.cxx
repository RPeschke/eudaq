#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include "eudaq/MultiFileReader.hh"
#include "eudaq/readAndProcessDataTemplate.h"
#include <iostream>
#include "eudaq/EventSynchronisationDetectorEvents.hh"

#include "eudaq/RawDataEvent.hh"
#include "eudaq/Platform.hh"
#include "eudaq/Processor_batch.hh"
#include "eudaq/Processors.hh"
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

    Processor_batch batch;

    batch.pushProcessor(Processors::fileReader(op));
    batch.pushProcessor(Processors::ShowEventNR(1000));
    batch.pushProcessor(Processors::eventSelector(parsenumbers(events->Value())));
    batch.pushProcessor(Processors::fileWriter());
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

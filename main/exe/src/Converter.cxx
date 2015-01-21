#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include "eudaq/MultiFileReader.hh"
#include "eudaq/readAndProcessDataTemplate.h"
#include <iostream>
#include "eudaq/EventSynchronisationDetectorEvents.hh"

using namespace eudaq;
unsigned dbg = 0;


int main(int, char ** argv) {
  std::clock_t    start;

  start = std::clock();
  eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);

  auto events = ReadAndProcess<eudaq::FileWriter>::add_Command_line_option_EventsOfInterest(op);


  auto type = FileWriterFactory::add_Command_line_option_OutputTypes(op);
  auto opat = FileWriterFactory::add_Command_line_option_OutputPattern(op);
  op.ExtraHelpText(FileWriterFactory::Help_text());

  auto ipat = multiFileReader::add_Command_line_option_inputPattern(op);
  op.ExtraHelpText(multiFileReader::help_text());


  eudaq::OptionFlag async(op, "a", "nosync", "Disables Synchronization with TLU events");
  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
                                   "The minimum level for displaying log messages locally");
  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());

    ReadAndProcess<eudaq::FileWriter> readProcess(!async.Value());
    readProcess.setEventsOfInterest(parsenumbers(events->Value()));
    
    if (op.NumArgs() == 1)
    {
      readProcess.addFileReader(eudaq::Factory_file_reader(op.GetArg(0), ipat->Value()));

    }
    else
    {
      auto multiReader = std::unique_ptr<baseFileReader>(new multiFileReader());
      auto mReader = dynamic_cast<eudaq::multiFileReader*>(multiReader.get());
      mReader->addSyncAlgorithm(std::unique_ptr<SyncBase>(new syncToDetectorEvents(true)));
      for (size_t i = 0; i < op.NumArgs(); ++i) {
       mReader->addFileReader(eudaq::Factory_file_reader(op.GetArg(i), ipat->Value()));

      }

      readProcess.addFileReader(std::move(multiReader));
     
    }
    readProcess.setWriter(FileWriterFactory::Create(type->Value()));

    readProcess.SetParameter(TAGNAME_OUTPUTPATTER, opat->Value());

    readProcess.StartRun();
    readProcess.process();
    readProcess.EndRun();

  }
  catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  std::cout << "Time: " << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  if (dbg>0)std::cout << "almost done with Converter. exiting" << std::endl;
  return 0;
}

#include "eudaq/OptionParser.hh"
#include "eudaq/Logger.hh"
#include "eudaq/Processor_inspector.hh"
#include "eudaq/Processor_batch.hh"
#include "eudaq/Processors.hh"
#include "eudaq/baseFileReader.hh"

using namespace eudaq;






int main(int /*argc*/, const char ** argv) {


  std::clock_t    start = std::clock();
  eudaq::OptionParser op("EUDAQ Configuration Extractor", "1.0", "", 1);


  FileReaderFactory::addComandLineOptions(op);

  op.ExtraHelpText(FileReaderFactory::Help_text());



  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
                                   "The minimum level for displaying log messages locally");

  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());


    Processor_batch batch;
    for (size_t i = 0; i < op.NumArgs(); ++i) {
      batch >> Processors::fileReader(fileConfig(op.GetArg(i)));

    }

    batch >> Processors::eventSelector({ 0, 1 }, true);
    batch >> ADD_LAMBDA_PROZESSOR1(ev) {
      if (ev.IsBORE()) {
        eudaq::Configuration conf(ev.GetTag("CONFIG"));
        conf.Save(std::cout);
      }
    };

    batch.init();
    batch.run();
    batch.end();

  } catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  return 0;
}

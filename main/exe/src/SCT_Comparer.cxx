#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include <iostream>

#include "eudaq/ProcessorBase.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/readAndProcessDataTemplate.h"

#include "eudaq/SCT_defs.hh"
#include "eudaq/Processor_inspector.hh"
#include "eudaq/Processor_batch.hh"

using namespace eudaq;
unsigned dbg = 0;
// class printOut : public Processor_Inspector{
// public:
//   printOut(Parameter_ref conf) :Processor_Inspector(conf){}
//   virtual ReturnParam inspecktEvent(const Event& ev) { 
//     
//     ev.Print(std::cout);
//     return ProcessorBase::sucess;
//   };
// };
// std::string printOutName(){
//   return "printOut";
// }
// 
// RegisterProcessor(printOut, printOutName());

ProcessorBase::ReturnParam inspecktEvent(const Event& ev) {
   
   ev.Print(std::cout);
   return ProcessorBase::sucess;
 };
int main(int, char ** argv) {
/*



  std::cout << ProConfig::Topic("hello") + ProConfig::Tag("tag", "value");
  std::clock_t    start;

  auto p = Configuration(ProConfig::Topic("hello") + ProConfig::Tag("tag", "value"), "hello");
  auto tag = p.Get("tag", "nix");
  start = std::clock();
  eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);

  auto events = ReadAndProcess<eudaq::FileWriter>::add_Command_line_option_EventsOfInterest(op);

  auto sct = std::unique_ptr<eudaq::Option<std::string>>(new eudaq::Option<std::string>(op, "s", "sct", "", "string", "reference ascci file"));


  //FileReaderFactory::addComandLineOptions(op);

  op.ExtraHelpText(FileReaderFactory::Help_text());


  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
    "The minimum level for displaying log messages locally");






  try {




    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());

    if (op.NumArgs() != 1)
    {
      EUDAQ_THROW("not the right amount of imput parameters");
    }
    if (sct->Value().empty())
    {
      EUDAQ_THROW("no sct ref file");
    }

  auto pro = std::unique_ptr<Processor_batch>(new Processor_batch(ProcessorConf("name")));

  auto && eee= [](const Event& ev) {

    ev.Print(std::cout);
    return ProcessorBase::sucess;

  };
 // pro->pushProcessor(make_Processor1(std::move(eee)));
  auto t = processor_T< decltype(eee)>(std::move(eee));
//   auto t = make_p(
//     [](const Event& ev) {
// 
//     ev.Print(std::cout);
//     return ProcessorBase::sucess;
// 
//   }
//   
//   );

  pro->pushProcessor(make_Processor(ProcessorConf("asda"),
    
    [](const Event& ev) {

    ev.Print(std::cout);
    return ProcessorBase::sucess;

  }
    ));
  pro->pushProcessor(make_Processor(

    [](const Event& ev) {

    ev.Print(std::cout);
    return ProcessorBase::sucess;

  }
  ));
  pro->pushProcessor(make_Processor(

 &inspecktEvent
  ));


  }
  catch (...) {
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
  std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  if (dbg > 0)std::cout << "almost done with Converter. exiting" << std::endl;
*/
  return 0;
}
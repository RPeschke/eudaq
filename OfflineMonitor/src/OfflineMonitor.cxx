#include "eudaq/FileReader.hh"
#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
//#include "eudaq/counted_ptr.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include <iostream>
#include "../inc/makeCorrelations.h"
#include "eudaq/PluginManager.hh"
#include "eudaq/MuliFileReader.h"

using namespace eudaq;
unsigned dbg = 0; 


int main(int, char ** argv) {
	std::clock_t    start;

	start = std::clock();
  eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);
  eudaq::Option<std::string> ipat(op, "i", "inpattern", "../data/run$6R.raw", "string", "Input filename pattern");
  eudaq::Option<std::string> opat(op, "o", "outpattern", "test$6R$X", "string", "Output filename pattern");
  eudaq::Option<std::string> confFile(op, "c", "confFile", "..\\conf\\configuration.xml", "string", "load the file that contains all the information about the correlations plots");
  eudaq::OptionFlag sync(op, "s", "synctlu", "Resynchronize subevents based on TLU event number");
  eudaq::Option<size_t> syncEvents(op, "n" ,"syncevents",0,"size_t","Number of events that need to be synchronous before they are used");
  eudaq::Option<long> syncDelay(op, "d" ,"longDelay",20,"unsigned long long","us time long time delay");
  eudaq::Option<size_t> skipEvents(op, "k" ,"skipEvents",0,"size_t","Number of events to skip");
  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
      "The minimum level for displaying log messages locally");
  op.ExtraHelpText("Available output types are: " + to_string(eudaq::FileWriterFactory::GetTypes(), ", "));

  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());
	std::cout<<"syncEvents"<<syncEvents.Value()<<std::endl;
	eudaq::multiFileReader reader;
	for (size_t i = 0; i < op.NumArgs(); ++i) {

		reader.addFileReader(op.GetArg(i), ipat.Value());
	}
      mCorrelations correlator;
	  correlator.open_confFile(confFile.Value().c_str());
	  correlator.SetFilePattern(opat.Value());
	  correlator.setRunNumber(reader.RunNumber());
	  correlator.open_outFile();
	  correlator.createHistograms();
	  
//      writer->SetFilePattern(opat.Value());
 //     writer->StartRun(reader.RunNumber());
	  int event_nr=0;
      do {
  

          if (!correlator.ProcessDetectorEvent(reader.GetDetectorEvent()))
          {
			  break;
          }
         
      
		  ++event_nr;
		  if (event_nr%1000==0)
		  {
			 
			  			  auto ev=reader.GetDetectorEvent().GetSubEvent<Event>(0);
				  		  std::cout <<ev->GetSubType()<<":  "<<ev->GetEventNumber()<<"   ";
						  auto ev1=reader.GetDetectorEvent().GetSubEvent<Event>(1);
						  std::cout <<ev1->GetSubType()<<":  "<<ev1->GetEventNumber()<<"   ";			  
						  auto ev2=reader.GetDetectorEvent().GetSubEvent<Event>(2);
						  std::cout <<ev->GetSubType()<<" countedtrigger:  "<<eudaq::PluginManager::GetTriggerID(*ev2)<<"   GetEventNumber:" <<ev->GetEventNumber();

	
			
			  std::cout <<std::endl;
		  }
        
      } while (reader.NextEvent(skipEvents.Value()));
     correlator.savePlotsAsPicture();
    
  } catch (...) {
	    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

  return 0;
}

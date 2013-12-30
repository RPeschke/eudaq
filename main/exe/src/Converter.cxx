#include "eudaq/FileReader.hh"
#include "eudaq/FileWriter.hh"
#include "eudaq/OptionParser.hh"
//#include "eudaq/counted_ptr.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"

#include <iostream>

using namespace eudaq;
unsigned dbg = 0; 

std::vector<unsigned> parsenumbers(const std::string & s) {
  std::vector<unsigned> result;
  std::vector<std::string> ranges = split(s, ",");
  for (size_t i = 0; i < ranges.size(); ++i) {
    size_t j = ranges[i].find('-');
    if (j == std::string::npos) {
      unsigned v = from_string(ranges[i], 0);
      result.push_back(v);
    } else {
      long min = from_string(ranges[i].substr(0, j), 0);
      long max = from_string(ranges[i].substr(j+1), 0);
      if (j == 0 && max == 1) {
        result.push_back((unsigned)-1);
      } else if (j == 0 || j == ranges[i].length()-1 || min < 0 || max < min) {
        EUDAQ_THROW("Bad range");
      } else {
        for (long n = min; n <= max; ++n) {
          result.push_back(n);
        }
      }
    }
  }
  return result;
}

int main(int, char ** argv) {
	std::clock_t    start;

	start = std::clock();
  eudaq::OptionParser op("EUDAQ File Converter", "1.0", "", 1);
  eudaq::Option<std::string> type(op, "t", "type", "native", "name", "Output file type");
  eudaq::Option<std::string> events(op, "e", "events", "", "numbers", "Event numbers to convert (eg. '1-10,99' default is all)");
  eudaq::Option<std::string> ipat(op, "i", "inpattern", "../data/run$6R.raw", "string", "Input filename pattern");
  eudaq::Option<std::string> opat(op, "o", "outpattern", "test$6R$X", "string", "Output filename pattern");
  eudaq::OptionFlag sync(op, "s", "synctlu", "Resynchronize subevents based on TLU event number");
  eudaq::Option<size_t> syncEvents(op, "n" ,"syncevents",1000,"size_t","Number of events that need to be synchronous before they are used");
  eudaq::Option<unsigned long long> syncDelay(op, "d" ,"longDelay",20,"unsigned long long","us time long time delay");
  eudaq::Option<std::string> level(op, "l", "log-level", "INFO", "level",
      "The minimum level for displaying log messages locally");
  op.ExtraHelpText("Available output types are: " + to_string(eudaq::FileWriterFactory::GetTypes(), ", "));
  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL(level.Value());
    std::vector<unsigned> numbers = parsenumbers(events.Value());
    for (size_t i = 0; i < op.NumArgs(); ++i) {
      eudaq::FileReader reader(op.GetArg(i), ipat.Value(), sync.IsSet(),syncEvents.Value(),syncDelay.Value());
      std::shared_ptr<eudaq::FileWriter> writer(FileWriterFactory::Create(type.Value()));
      writer->SetFilePattern(opat.Value());
      writer->StartRun(reader.RunNumber());
	  int event_nr=0;
      do {
        if (reader.GetDetectorEvent().IsBORE() || reader.GetDetectorEvent().IsEORE() || numbers.empty() ||
            std::find(numbers.begin(), numbers.end(), reader.GetDetectorEvent().GetEventNumber()) != numbers.end()) {
          writer->WriteEvent(reader.GetDetectorEvent());
          if(dbg>0)std::cout<< "writing one more event" << std::endl;
		  ++event_nr;
		  if (event_nr%1000==0)
		  {
			  std::cout<<"Processing event "<< event_nr<<std::endl;
		  }
        }
      } while (reader.NextEvent());
      if(dbg>0)std::cout<< "no more events to read" << std::endl;
    }
  } catch (...) {
	    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    return op.HandleMainException();
  }
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  if(dbg>0)std::cout<< "almost done with Converter. exiting" << std::endl;
  return 0;
}

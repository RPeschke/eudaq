#ifndef Processors_h__
#define Processors_h__
#include <vector>
#include <memory>
#include "eudaq/Platform.hh"
namespace eudaq {
class ProcessorBase;
class fileConfig;
namespace Processors {
using processor_up = std::unique_ptr<eudaq::ProcessorBase>;
DLLEXPORT processor_up eventSelector(const std::vector<unsigned>& eventsOfIntresst, bool doBore = true, bool doEORE = true);
DLLEXPORT processor_up ShowEventNR(size_t repetition);
DLLEXPORT processor_up fileReader(const eudaq::fileConfig & );
DLLEXPORT processor_up fileWriter();

}

}

#endif // Processors_h__

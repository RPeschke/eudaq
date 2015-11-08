#ifndef Processors_h__
#define Processors_h__
#include <vector>
#include <memory>
#include "eudaq/Platform.hh"
#include "EventSynchronisationBase.hh"
namespace eudaq {
class ProcessorBase;
class fileConfig;
namespace Processors {
using processor_up = std::unique_ptr<eudaq::ProcessorBase>;
DLLEXPORT processor_up eventSelector(const std::vector<unsigned>& eventsOfIntresst, bool doBore = true, bool doEORE = true);
DLLEXPORT processor_up ShowEventNR(size_t repetition);
DLLEXPORT processor_up fileReader(const eudaq::fileConfig &);
DLLEXPORT processor_up fileWriter();
DLLEXPORT processor_up merger(const SyncBase::MainType& type_, SyncBase::Parameter_ref param_ = SyncBase::Parameter_t());
DLLEXPORT processor_up dataReciver(const std::string& listAdrrs);



}

}

#endif // Processors_h__

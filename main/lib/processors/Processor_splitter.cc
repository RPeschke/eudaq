#include "eudaq/Processor_splitter.hh"

#include "eudaq/PluginManager.hh"
#include "eudaq/DetectorEvent.hh"
#include <string>
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  ReturnParam eudaq::Processor_splitter::ProcessEvent(ConnectionName_ref name, event_sp ev)
  {
    auto ret = ProcessorBase::sucess;

    if (ev->IsPacket())
    {
      for (size_t i = 0; i < PluginManager::GetNumberOfROF(*ev);++i)
      {
           ret= ProcessorBaseEvent(name, PluginManager::ExtractEventN(ev, i));
           if (ret!=ProcessorBase::sucess)
           {
             return ret;
           }
      }
      if (ev->get_id()==DetectorEvent::eudaq_static_id())
      {
        auto det = dynamic_cast<DetectorEvent*>(ev.get());
        det->clearEvents();
        ret = ProcessorBaseEvent(name, std::move(ev));
        if (ret != ProcessorBase::sucess)
        {
          return ret;
        }
      }

      return ProcessorBase::sucess;
    }
    
   return ProcessorBaseEvent(name, std::move(ev));

  }



  ReturnParam Processor_splitter::ProcessorBaseEvent(ConnectionName_ref name, event_sp ev)
  {

    auto id=PluginManager::getUniqueIdentifier(*ev);
    

    auto newName =concatenate(name ,id);


   return ProcessNext(newName,std::move(ev));

  }

  void Processor_splitter::initialize()
  {

  }


  Processor_splitter::Processor_splitter(Parameter_ref conf) :Processor_N_x_M(conf)
  {

  }

}
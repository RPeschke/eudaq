#include "eudaq/Processor_splitter.hh"

#include "eudaq/PluginManager.hh"
#include "eudaq/DetectorEvent.hh"
#include <string>
namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  RegisterProcessor(Processor_splitter, ProcessorNames::splitter());
  ReturnParam eudaq::Processor_splitter::ProcessorEvent(ConnectionName_ref name, event_sp ev)
  {
    auto ret = ProcessorBase::sucess;
    auto name1 = concatenate(name,ev->GetTag("__fileReader", "null"));

    if (ev->IsPacket())
    {
      for (size_t i = 0; i < PluginManager::GetNumberOfROF(*ev);++i)
      {
           ret= ProcessorBaseEvent(name1, PluginManager::ExtractEventN(ev, i));
           if (ret!=ProcessorBase::sucess)
           {
             return ret;
           }
      }
      if (ev->get_id()==DetectorEvent::eudaq_static_id())
      {
        auto det = dynamic_cast<DetectorEvent*>(ev.get());
        det->clearEvents();
        ret = ProcessorBaseEvent(name1, ev);
        if (ret != ProcessorBase::sucess)
        {
          return ret;
        }
      }

      return ProcessorBase::sucess;
    }
    
   return ProcessorBaseEvent(name1, ev);

  }

  Processor_up Processor_splitter::CreateInterface(ConnectionName_ref name, Parameter_ref conf)
  {

  }

  ReturnParam Processor_splitter::ProcessorBaseEvent(ConnectionName_ref name, event_sp ev)
  {

    auto id=PluginManager::getUniqueIdentifier(*ev);
    

    auto newName =concatenate(name ,id);


   return ProcessNext(newName, ev);

  }

  void Processor_splitter::initialize(Configuration_ref conf)
  {

  }


}
#ifndef ProcessorBase_h__
#define ProcessorBase_h__



#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/factory.hh"
#include <memory>

#define RegisterProcessor(derivedClass,ID) registerClass(eudaq::ProcessorBase,derivedClass,ID)



namespace eudaq{
  
  class OptionParser;
  class ProcessorBase;
 


  using Processor_sp = std::shared_ptr < ProcessorBase >;
  class DLLEXPORT ProcessorBase{
  public:
    using MainType = std::string;
    using Parameter_t = std::string;
    using Parameter_ref = const Parameter_t&;


    ProcessorBase(Parameter_ref name);
    virtual void init() =0;
    virtual void ProcessorEvent(event_sp ev) =0;
    virtual void end() =0;


    
    virtual ProcessorBase* getProcessor(const std::string& name ="")=0;
    
    virtual void AddProcessor(ProcessorBase *next,const  std::string& name ="") =0;
    
    virtual std::string getName() =0;
    virtual void print(std::ostream& os)=0;
  
  protected:
    Parameter_t m_conf;
  };

  
  class DLLEXPORT ProcessorFactory{
  public:


    static std::unique_ptr<ProcessorBase> create(ProcessorBase::MainType type, ProcessorBase::Parameter_ref  param);



    static void addComandLineOptions(eudaq::OptionParser & op);
    static std::string Help_text();
    static ProcessorBase::Parameter_t getDefault();

  private:

    class Impl;
    static Impl& getImpl();

  };

  
}
#endif // ProcessorBase_h__

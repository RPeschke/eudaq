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
  using Processor_up = std::unique_ptr < ProcessorBase > ;

  using Processor_rp =ProcessorBase*;  //reference pointer 

  class DLLEXPORT ProcessorBase{
  public:
    enum ReturnParam:int
    {
      sucess,
      ret_error,
      stop,
      busy_retry,
      busy_skip,
      skip_event

    };
    using MainType = std::string;
    using Parameter_t = std::string;
    using Parameter_ref = const Parameter_t&;
    using ConnectionName_t=std::string;
    using ConnectionName_ref = const ConnectionName_t &;


    ProcessorBase(Parameter_ref name);
    virtual ~ProcessorBase() {};
    virtual void init() =0;
    virtual ReturnParam ProcessorEvent(event_sp ev) = 0;
    virtual void end() =0;


    
    virtual Processor_rp getProcessor(ConnectionName_ref name = "") = 0;
    
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") = 0;
    
    virtual std::string getName() =0;
    virtual void print(std::ostream& os)=0;
    virtual void pushProducer(Processor_up processor) =0;
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
  template<typename T>
  ProcessorBase::ConnectionName_t concatenate(ProcessorBase::ConnectionName_ref first, T second){
    return first + std::to_string(second);
  }
  template<>
  ProcessorBase::ConnectionName_t concatenate<std::string>(ProcessorBase::ConnectionName_ref first, std::string second);
  class DLLEXPORT ProcessorNames{
  public:
    using  Name_t = ProcessorBase::MainType;

    static Name_t batch();
    static Name_t buffer();
    static Name_t multi_buffer();
    static Name_t busy_test();
    static Name_t file_reader();
    static Name_t file_writer();
    static Name_t show_event_nr();
    static Name_t events_of_intresst();
    static Name_t splitter(); 
    static Name_t merger();
    static Name_t Parallel_file_reader();
    static Name_t Parallel_processor();
  };
}
#endif // ProcessorBase_h__

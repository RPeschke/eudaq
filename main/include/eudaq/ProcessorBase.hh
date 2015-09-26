#ifndef ProcessorBase_h__
#define ProcessorBase_h__



#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/factory.hh"
#include <memory>
#include <map>
#include <string>

//#define RegisterProcessor(derivedClass,ID) registerClass(eudaq::ProcessorBase,derivedClass,ID)



namespace eudaq{
  
  class OptionParser;
  class ProcessorBase;
  class ProcessorOptions;


  using Processor_sp = std::shared_ptr < ProcessorBase >; 
  using Processor_up = std::unique_ptr < ProcessorBase > ;

  using Processor_rp =ProcessorBase*;  //reference pointer 

  using ProcessorOptions_up = std::unique_ptr < ProcessorOptions > ;
  using ProcessorOptions_rp =  ProcessorOptions*;

  class ProcessorOptions{
  public:
    ProcessorOptions(std::string name):m_name(std::move(name)){ }
    std::string getName() const;
    void setName(const std::string & name);
    void addNext(ProcessorOptions_up next);

    ProcessorOptions_rp getByName(const std::string& name) const;
    ProcessorOptions_rp getNext() const;

    void SetTag(const std::string & name, const std::string & val);
    std::string GetTag(const std::string & name, const std::string & def = "") const;
    static ProcessorOptions_up create(std::string name)
    {
      return ProcessorOptions_up(new ProcessorOptions(std::move(name)));
    }
  private:
    typedef std::map<std::string, std::string> map_t;
    map_t m_tags;
    ProcessorOptions_up m_next;
    std::string m_name;
  };
  
  DLLEXPORT ProcessorOptions_up operator+(ProcessorOptions_up a, ProcessorOptions_up b);
  
class DLLEXPORT ProcessorConf{
public:
explicit ProcessorConf(const std::string& name):m_name(name){}
explicit ProcessorConf(const char* name):m_name(name){}
std::string getName() const{
  return m_name;
}
ProcessorConf& setPos(int pos){
  m_pos = pos;
  return *this;
}
int getPos()const {
  return m_pos;
}
private:
std::string m_name;
int m_pos = 0;
};


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
    using Parameter_t = ProcessorConf;
    using Parameter_ref = const Parameter_t&;
    using ConnectionName_t=std::string;
    using ConnectionName_ref = const ConnectionName_t &;
  using Configuration_t = std::string;
  using Configuration_ref = const Configuration_t&;
    ProcessorBase(Parameter_ref name);
    virtual ~ProcessorBase() {};
  virtual void init() = 0;
    virtual ReturnParam ProcessEvent(event_sp ev) = 0;
    virtual void end() =0;


    
    virtual Processor_rp getProcessor(ConnectionName_ref name = "") = 0;
    
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") = 0;
    
    std::string getName();
    virtual void print(std::ostream& os)=0;
    virtual void pushProcessorBase(Processor_up processor) =0;

    template <typename T> 
    T* pushProcessor(std::unique_ptr<T> processor){
      T* ret = processor.get();
      pushProcessorBase(std::move(processor));
      return ret;
    }

template <class T, class... Args>
    T* pushNewProcessor(Args&&... args){
      auto processor=__make_unique<T>(args...);
      return pushProcessor(std::move(processor));
    }
    virtual void clearProcessor() = 0;
  protected:
    Parameter_t m_conf;
    std::string m_name;
  };


 
  

  template<typename T>
  ProcessorBase::ConnectionName_t concatenate(ProcessorBase::ConnectionName_ref first, T second){
    return first + std::to_string(second);
  }
  template<>
  ProcessorBase::ConnectionName_t concatenate<std::string>(ProcessorBase::ConnectionName_ref first, std::string second);

  class DLLEXPORT ProConfig{
  public:
    using Config = ProcessorBase::Configuration_t;
    using ConfigInput = ProcessorBase::Configuration_t;

    static Config Topic(const std::string& name);
    static Config Tag(const std::string& tagName, const std::string& tagValue);
    static Config ProcessorName(const std::string& Name);
    static Config ProcessorType(const std::string& Name);
    static Config Filename(const std::string& fileName);
    static Config ProcessorParallelType(const std::string& Name);
    static Config ProcessorParallelPos(size_t pos);
    static Config getConfig_configuration(const std::string& name, const std::string& sectionANDTAGS);
    static Config getConfig_ConcatSectionsTag(const std::string& section, const std::string& TAGS);
    static Config eventSelection_configuration(const std::string& name, const std::string& events, bool doBore=false,bool doEore=false);

    static std::string getTag(const ConfigInput& conf, const std::string& section, const std::string& tag,const std::string& def);
    static std::string getFilename(const ConfigInput& conf,const std::string& section,const std::string& def);
    static std::string  getProcessorName(const ConfigInput& conf);
    static std::string  getProcessorType(const ConfigInput& conf);
    static std::string  getProcessorParallelType(const ConfigInput& conf);
    static size_t getProcessorParallelPos(Config conf);


  };


}
#endif // ProcessorBase_h__

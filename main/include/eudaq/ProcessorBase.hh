#ifndef ProcessorBase_h__
#define ProcessorBase_h__



#include "eudaq/Platform.hh"
#include "eudaq/Event.hh"
#include "eudaq/Configuration.hh"

#include <memory>
#include <map>
#include <string>





namespace eudaq {


class ProcessorBase;
class ProcessorOptions;


using Processor_sp = std::shared_ptr < ProcessorBase >;
using Processor_up = std::unique_ptr < ProcessorBase >;

using Processor_rp = ProcessorBase*;  //reference pointer 

using ProcessorOptions_up = std::unique_ptr < ProcessorOptions >;
using ProcessorOptions_rp = ProcessorOptions*;


class DLLEXPORT ProcessorConf {
public:
  explicit ProcessorConf(const std::string& name,int pos =0 ) :m_name(name),m_pos(pos) {}
  explicit ProcessorConf(const char* name, int pos = 0) :m_name(name), m_pos(pos) {}
  std::string getName() const {
    return m_name;
  }
  ProcessorConf& setPos(int pos) {
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


class DLLEXPORT ProcessorBase {
public:
  enum ReturnParam :int {
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
  using ConnectionName_t = size_t;
  using ConnectionName_ref = const ConnectionName_t &;


  ProcessorBase(Parameter_ref name);
  virtual ~ProcessorBase() {};
  virtual void init() = 0;
  virtual ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con) = 0;
  virtual void end() = 0;


  void AddNextProcessor(Processor_rp next);
  
protected:
  ReturnParam processNext(event_sp ev, ConnectionName_ref con);
  Parameter_t m_conf;
private:
  Processor_rp m_next = nullptr;
};

DLLEXPORT ProcessorBase::ConnectionName_t default_connection();
DLLEXPORT ProcessorBase::ConnectionName_t random_connection();

template<class T, class... Args>
Processor_up make_Processor(Args&&... args) {
  auto p = new T(std::forward<Args>(args)...);
  return Processor_up(p);
}

}
#endif // ProcessorBase_h__

#ifndef Processor_inspector_h__
#define Processor_inspector_h__

#include "eudaq/ProcessorBase.hh"
#include "eudaq/Platform.hh"
#include <type_traits> 
#include <utility>



#define ADD_LAMBDA_PROZESSOR0() hidden::___processor_dummy_void() + []()
#define ADD_LAMBDA_PROZESSOR1(event_ref) hidden::___processor_dummy_event() + [](const eudaq::Event& event_ref) 
#define ADD_LAMBDA_PROZESSOR2(event_ref,COnnectionName) hidden::___processor_dummy_full() + [](const eudaq::Event& event_ref, size_t COnnectionName)

namespace eudaq {



  class DLLEXPORT Processor_Inspector :public ProcessorBase {
  public:


    Processor_Inspector(Parameter_ref conf);
    Processor_Inspector();

    void init() override {}
    void end() override {}
    virtual ReturnParam inspecktEvent(const Event&, ConnectionName_ref con) = 0;



    ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con) override;
  };

  template <typename T>
  class  processor_T : public Processor_Inspector {
  public:
    processor_T(Parameter_ref conf, T&& proc_) :Processor_Inspector(conf), m_proc(std::forward<T>(proc_)) {}

    processor_T(T&& proc_) : m_proc(std::forward<T>(proc_)) {}
    

    virtual ReturnParam inspecktEvent(const Event& e, ConnectionName_ref con) override {
      m_proc(e, con);
      return ProcessorBase::sucess;
    }
  private:
    T m_proc;

  };

  template <typename T>
  class  processor_T_void : public Processor_Inspector {
  public:
    processor_T_void(Parameter_ref conf, T&& proc_) :Processor_Inspector(conf), m_proc(std::forward<T>(proc_)) {}

    processor_T_void(T&& proc_) : m_proc(std::forward<T>(proc_)) {}


    virtual ReturnParam inspecktEvent(const Event& e, ConnectionName_ref con) override {
      m_proc();
      return ProcessorBase::sucess;
    }
  private:
    T m_proc;

  };
  template <typename T>
  class  processor_T_event: public Processor_Inspector {
  public:
    processor_T_event(Parameter_ref conf, T&& proc_) :Processor_Inspector(conf), m_proc(std::forward<T>(proc_)) {}

    processor_T_event(T&& proc_) : m_proc(std::forward<T>(proc_)) {}


    virtual ReturnParam inspecktEvent(const Event& e, ConnectionName_ref con) override {
      m_proc(e);
      return  ProcessorBase::sucess;
    }
  private:
    T m_proc;

  };

  template <typename T>
  Processor_up make_Processor(eudaq::ProcessorBase::Parameter_ref conf, T&& proc_) {
    return Processor_up(new processor_T<T>(conf, std::forward<T>(proc_)));
  }
  template <typename T>
  Processor_up make_Processor(T&& proc_) {

    return  Processor_up(new processor_T<T>(std::forward<T>(proc_)));
  }

  template <typename T>
  Processor_up make_Processor_void(eudaq::ProcessorBase::Parameter_ref conf, T&& proc_) {
    return Processor_up(new processor_T_void<T>(conf, std::forward<T>(proc_)));
  }
  template <typename T>
  Processor_up make_Processor_void(T&& proc_) {

    return  Processor_up(new processor_T_void<T>(std::forward<T>(proc_)));
  }
  
  
  template <typename T>
  Processor_up make_Processor_event(eudaq::ProcessorBase::Parameter_ref conf, T&& proc_) {
    return Processor_up(new processor_T_void<T>(conf, std::forward<T>(proc_)));
  }
  template <typename T>
  Processor_up make_Processor_event(T&& proc_) {

    return  Processor_up(new processor_T_event<T>(std::forward<T>(proc_)));
  }
  namespace hidden {
  class ___processor_dummy_void {

  };
  template <typename T>
  Processor_up operator+(const ___processor_dummy_void &, T&& t) {
    return make_Processor_void(std::forward<T>(t));
  }

  class ___processor_dummy_event {

  };
  template <typename T>
  Processor_up operator+(const ___processor_dummy_event &, T&& t) {
    return make_Processor_event(std::forward<T>(t));
  }

  class ___processor_dummy_full {

  };
  template <typename T>
  Processor_up operator+(const ___processor_dummy_full &, T&& t) {
    return make_Processor(std::forward<T>(t));
  }
  }
}
#endif // Processor_inspector_h__

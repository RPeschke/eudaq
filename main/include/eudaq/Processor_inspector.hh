#ifndef Processor_inspector_h__
#define Processor_inspector_h__

#include "eudaq/Processor.hh"
#include "eudaq/Platform.hh"
#include <type_traits> 
#include <utility>



namespace eudaq {



  class DLLEXPORT Processor_Inspector :public Processor {
  public:


    Processor_Inspector(Parameter_ref conf);
    Processor_Inspector();

    virtual ReturnParam inspecktEvent(const Event&) = 0;



    ReturnParam ProcessEvent(event_sp ev) override;
  };

  template <typename T>
  class  processor_T : public Processor_Inspector {
  public:
    processor_T(Parameter_ref conf, T&& proc_) :Processor_Inspector(conf), m_proc(std::forward<T>(proc_)) {}

    processor_T(T&& proc_) : m_proc(std::forward<T>(proc_)) {}
    

    virtual ReturnParam inspecktEvent(const Event& e) override {
      
      return m_proc(e);
    }
  private:
    T m_proc;

  };
  template <typename T>
  processor_T<T> make_p(T&& proc_) {
    return processor_T<T>(std::forward<T>(proc_));
  }
  template <typename T>
  std::unique_ptr<processor_T<T>> make_Processor(eudaq::ProcessorBase::Parameter_ref conf, T&& proc_) {
    return std::unique_ptr<processor_T<T>>(new processor_T<T>(conf, std::forward<T>(proc_)));
  }
  template <typename T>
  std::unique_ptr<processor_T<T>> make_Processor(T&& proc_) {

    return  std::unique_ptr<processor_T<T>>( new processor_T<T>(  std::forward<T>(proc_)));
  }
}
#endif // Processor_inspector_h__

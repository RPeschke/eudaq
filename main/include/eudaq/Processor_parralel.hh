#ifndef Processor_parralel_h__
#define Processor_parralel_h__

#include "eudaq/Processor_N_2_N.hh"
namespace eudaq{
  template <typename T>
  class Processor_parallel :public Processor_N_2_N{
  public:
    Processor_parallel(Parameter_ref conf) :Processor_N_2_N(conf){}
    virtual ~Processor_parallel(){}
    

    virtual Processor_up CreateInterface(ConnectionName_ref name, Parameter_ref conf)
    {
      ProcessorConf p(name);
      p.setPos(m_interfaces->size() + 1);
      return Processor_up(new T(p));
    }
    virtual std::string getName()
    {
      return std::string("multi") + m_conf.getName();
    }

    virtual void print(std::ostream& os)
    {
      os << getName();
    }


  public:
    std::string m_type;
    
  };
}
#endif // Processor_parralel_h__

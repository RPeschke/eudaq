#ifndef Processor_parralel_h__
#define Processor_parralel_h__

#include "eudaq/Processor_N_2_N.hh"
namespace eudaq{
  class Processor_parallel :public Processor_N_2_N{
  public:
    Processor_parallel(Parameter_ref conf) :Processor_N_2_N(conf){}
    virtual ~Processor_parallel(){}
    virtual void finish();
    virtual void initialize(Configuration_ref conf) override;
    virtual Processor_up CreateInterface(ConnectionName_ref name, Parameter_ref conf);
    virtual std::string getName();
    virtual void print(std::ostream& os);

  public:
    std::string m_type;
    
  };
}
#endif // Processor_parralel_h__

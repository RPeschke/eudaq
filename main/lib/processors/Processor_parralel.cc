#include "eudaq/Processor_N_2_N.hh"


namespace eudaq{
  class Processor_parallel :public Processor_N_2_N{
  public:
    Processor_parallel(Parameter_ref conf) :Processor_N_2_N(conf){}
    virtual ~Processor_parallel(){}
    virtual void multiEnd() ;
    virtual void Initilize() ;
    virtual Processor_up CreateProcessor(ConnectionName_ref name, Parameter_ref conf) ;
    virtual std::string getName();
    virtual void print(std::ostream& os);


  };
  RegisterProcessor(Processor_parallel, ProcessorNames::Parallel_processor());
  void Processor_parallel::multiEnd()
  {

  }

  void Processor_parallel::Initilize()
  {

  }

  Processor_up Processor_parallel::CreateProcessor(ConnectionName_ref name, Parameter_ref conf)
  {
    return ProcessorFactory::create(conf, name);
  }

  std::string Processor_parallel::getName()
  {
    return std::string("multi")+m_conf;
  }

  void Processor_parallel::print(std::ostream& os)
  {
    os << getName();
  }



}
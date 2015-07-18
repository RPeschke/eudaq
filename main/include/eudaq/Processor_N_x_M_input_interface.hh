#ifndef Processor_splitter_interface_h__
#define Processor_splitter_interface_h__
#include "eudaq/ProcessorBase.hh"

namespace eudaq{
  class Processor_N_x_M;
  class Processor_N_x_M_input_interface :public ProcessorBase{
  public:
    Processor_N_x_M_input_interface(Parameter_ref name);
    virtual ~Processor_N_x_M_input_interface() {};
	virtual void init(){}
    virtual ReturnParam ProcessEvent(event_sp ev);
    virtual void end(){}
    virtual void clearProcessor(){}


    virtual ProcessorBase* getProcessor(ConnectionName_ref name = "");

    virtual void AddProcessor(ProcessorBase *next, ConnectionName_ref name = "") ;

    virtual std::string getName();
    virtual void print(std::ostream& os);
    virtual void pushProcessorBase(Processor_up processor) {}
  private:

    Processor_N_x_M* m_baseProcessor = nullptr;
    std::string m_name, m_baseName;
  };

}

#endif // Processor_splitter_interface_h__

#ifndef Processor_splitter_h__
#define Processor_splitter_h__

#include "eudaq/Processor_N_x_M.hh"

namespace eudaq{


  class Processor_splitter :public Processor_N_x_M{

  public:
    virtual ReturnParam ProcessorEvent(ConnectionName_ref name, event_sp ev) override;
    virtual void initialize(Configuration_ref conf);
    Processor_splitter(Parameter_ref);
    virtual ~Processor_splitter(){}
  private:
    Processor_up CreateInterface(ConnectionName_ref name, Parameter_ref conf);
    ReturnParam ProcessorBaseEvent(ConnectionName_ref name, event_sp ev);





  };
}

#endif // Processor_splitter_h__

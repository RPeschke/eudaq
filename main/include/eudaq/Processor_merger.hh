#ifndef Processor_merger_h__
#define Processor_merger_h__

#include "eudaq/Processor_N_x_M.hh"



namespace eudaq{



  class Processor_merger :public Processor_N_x_M{

    virtual ReturnParam ProcessorEvent(ConnectionName_ref name, event_sp ev) = 0;
    Processor_merger(Parameter_ref);
    virtual ~Processor_merger(){}
  };
}



#endif // Processor_merger_h__

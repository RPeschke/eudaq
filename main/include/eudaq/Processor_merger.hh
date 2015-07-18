#ifndef Processor_merger_h__
#define Processor_merger_h__

#include "eudaq/Processor_N_x_M.hh"



namespace eudaq{


  class SyncBase;
  class Processor_merger :public Processor_N_x_M{
  public:
    virtual ReturnParam ProcessEvent(ConnectionName_ref name, event_sp ev) ;
    Processor_merger(Parameter_ref);
    virtual ~Processor_merger(){}
    virtual void initialize();
    virtual void finish() override;
  private:
    std::map<ConnectionName_t, unsigned> m_map;
    unsigned m_counter = 0;
    std::unique_ptr < SyncBase > m_sync;
  };
}



#endif // Processor_merger_h__

#ifndef Processor_add2queue_h__
#define Processor_add2queue_h__




#include "eudaq/Processor.hh"



namespace eudaq{



  class  Processor_add2queue :public Processor{
  public:


    Processor_add2queue(Parameter_ref conf);


    virtual ReturnParam add2queue(event_sp& ev) = 0;



    ReturnParam ProcessEvent(event_sp ev) override;
    ReturnParam m_last_ret;
    enum status
    {
      configuring,
      running,
      stopping

    }m_status;

  private:

    void handelReturn(ReturnParam ret);
  };


}
#endif // Processor_add2queue_h__
#ifndef Processor_Buffer_h__
#define Processor_Buffer_h__
#include "Processor.hh"
#include <deque>



namespace eudaq{


  class Proccessor_Buffer :public Processor{
  public:
    Proccessor_Buffer(Parameter_ref conf);
    ~Proccessor_Buffer(){}
    virtual ReturnParam ProcessorEvent(event_sp ev) ;
	virtual void initialize(Configuration_ref conf) override;



  private:
    std::deque<event_sp> m_queue; // this is a vector to make it possible to access elements in the middle 
    size_t m_bufferSize = 100;        //Magic number for now 

  };

}


#endif // Processor_Buffer_h__

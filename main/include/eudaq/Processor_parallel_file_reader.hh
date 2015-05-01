#ifndef Processor_parallel_file_reader_h__
#define Processor_parallel_file_reader_h__
#include "eudaq/Processor_Parrallel_add2queue.hh"


namespace eudaq{

  class Processor_parallel_file :public Processor_Parrallel_add2queue{

  public:
    
    Processor_parallel_file(Parameter_ref conf);
    virtual ~Processor_parallel_file(){}
	virtual void initialize(Configuration_ref conf) override;
  virtual void finish() override;

    virtual ReturnParam ProcessorEvent(event_sp ev);
    virtual void print(std::ostream& os);
  private:
    Processor_up m_reader = nullptr;
  };

}

#endif // Processor_parallel_file_reader_h__

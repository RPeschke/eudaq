#ifndef ProcessorFileReader_h__
#define ProcessorFileReader_h__
#include "eudaq/baseFileReader.hh"
#include "eudaq/Processor_add2queue.hh"
namespace eudaq{
  class ProcessorFileReader : public Processor_add2queue
  {
  public:
    ProcessorFileReader(Parameter_ref conf);

    virtual ReturnParam add2queue(event_sp& ev);
    virtual void initialize(Configuration_ref conf);


    virtual void print(std::ostream& os);
  private:

    std::unique_ptr<baseFileReader> m_reader;
    bool m_first = true;
  };
}
#endif // ProcessorFileReader_h__

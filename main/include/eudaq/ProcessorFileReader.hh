#ifndef ProcessorFileReader_h__
#define ProcessorFileReader_h__
#include "eudaq/baseFileReader.hh"
#include "eudaq/Processor_add2queue.hh"
#include "eudaq/types/fileName.hh"
namespace eudaq{
  class ProcessorFileReader : public Processor_add2queue
  {
  public:
    ProcessorFileReader(Parameter_ref conf, const fileName& Fname, ConnectionName_ref con_);
    void setFileName(const fileName& fName);
    virtual ReturnParam add2queue(event_sp& ev);
    virtual void initialize() override;

  private:

    std::unique_ptr<baseFileReader> m_reader;
    bool m_first = true;
    fileName m_fName;
  };
}
#endif // ProcessorFileReader_h__

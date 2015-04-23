

#include "eudaq/Processor.hh"
#include "eudaq/baseFileReader.hh"

namespace eudaq{
  using ReturnParam =ProcessorBase::ReturnParam;

  class ProcessorFileReader : public Processor
  {
  public:
    ProcessorFileReader(Parameter_ref conf);

    virtual ReturnParam ProcessorEvent(event_sp ev);
   

    virtual std::string getName() override;
    virtual void print(std::ostream& os);
  private:

    std::unique_ptr<baseFileReader> m_reader;

  };


  RegisterProcessor(ProcessorFileReader, "fileReader");


  ProcessorFileReader::ProcessorFileReader(Parameter_ref conf) :Processor(conf)
  {
    m_reader =FileReaderFactory::create("../data/run000047_ - Kopie.raw");
  }


  std::string ProcessorFileReader::getName()
  {
   
    return m_conf;
  }


  void ProcessorFileReader::print(std::ostream& os)
  {
    std::cout << m_conf << std::endl;
  }


  ReturnParam ProcessorFileReader::ProcessorEvent(event_sp ev)
  {
    ReturnParam ret=sucess;
    ev = m_reader->getEventPtr();
    while (m_reader->NextEvent())
    {
    
      ret= ProcessNext(ev);
      if (ret==stop)
      {
        return stop;
      }
      ev = m_reader->getEventPtr();
    }
    ret = ProcessNext(ev);
    
   return stop;
  }



}
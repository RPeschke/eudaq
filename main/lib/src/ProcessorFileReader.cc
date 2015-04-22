

#include "eudaq/Processor.hh"
#include "eudaq/baseFileReader.hh"

namespace eudaq{

  class ProcessorFileReader : public Processor
  {
  public:
    ProcessorFileReader(Parameter_ref conf);

    virtual void ProcessorEvent(event_sp ev);
   

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


  void ProcessorFileReader::ProcessorEvent(event_sp ev)
  {
    ev = m_reader->getEventPtr();
    while (m_reader->NextEvent())
    {
    
      ProcessNext(ev);
      ev = m_reader->getEventPtr();
    }

    ProcessNext(ev);
  }



}
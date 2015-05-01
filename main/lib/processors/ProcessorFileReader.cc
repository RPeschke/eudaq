

#include "eudaq/baseFileReader.hh"
#include "eudaq/Processor_add2queue.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class ProcessorFileReader : public Processor_add2queue
  {
  public:
    ProcessorFileReader(Parameter_ref conf);

    virtual ReturnParam add2queue(event_sp& ev);
    virtual void initialize(Configuration_ref conf);

    virtual std::string getName() override;
    virtual void print(std::ostream& os);
  private:
   
    std::unique_ptr<baseFileReader> m_reader;
    bool m_first = true;
  };


  RegisterProcessor(ProcessorFileReader, ProcessorNames::file_reader());


  ProcessorFileReader::ProcessorFileReader(Parameter_ref conf) :Processor_add2queue(conf)
  {
    
    
  }


  std::string ProcessorFileReader::getName()
  {
    
    auto name = ProConfig::getProcessorName(m_conf);
    return ProConfig::getProcessorName(m_conf);
  }


  void ProcessorFileReader::print(std::ostream& os)
  {
    std::cout << m_conf << std::endl;
  }



  ReturnParam ProcessorFileReader::add2queue(event_sp& ev)
  {
    ReturnParam ret = sucess;
    if (m_first)
    {
      ev = m_reader->getEventPtr();
      m_first = false;
      ev->SetTag("__fileReader", getName());
      return sucess;
    }

    if (m_reader->NextEvent())
    {
      ev = m_reader->getEventPtr();
      ev->SetTag("__fileReader", getName());
      return sucess;

    }

    

    return stop;
  }



  void ProcessorFileReader::initialize(Configuration_ref conf)
  {
    m_reader = FileReaderFactory::create(ProConfig::getFilename(conf,getName(),""));
  }

}
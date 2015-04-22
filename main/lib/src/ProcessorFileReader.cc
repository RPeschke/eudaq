

#include "eudaq/ProcessorBase.hh"
#include "eudaq/baseFileReader.hh"

namespace eudaq{

  class ProcessorFileReader : public ProcessorBase
  {
  public:
    ProcessorFileReader(Parameter_ref conf);
    virtual void init() override;
    virtual void ProcessorEvent(event_sp ev) override;
    virtual void end()override;



    virtual ProcessorBase* getProcessor(std::string name = "") override;

    virtual void AddProcessor(ProcessorBase* next, std::string = "") override;

    virtual std::string getName() override;
    virtual void print(std::ostream& os);
  private:

    std::unique_ptr<baseFileReader> m_reader;
    ProcessorBase* m_next;
  };


  RegisterProcessor(ProcessorFileReader, "fileReader");


  ProcessorFileReader::ProcessorFileReader(Parameter_ref conf) :ProcessorBase(conf)
  {
    m_reader =FileReaderFactory::create("../data/run000145_.raw");
  }

  void ProcessorFileReader::init()
  {
    std::cout << "file reader: \n file name :" << m_conf << std::endl;
  }

  void ProcessorFileReader::end()
  {
    std::cout << "end File reader " << m_conf << std::endl;
  }

  std::string ProcessorFileReader::getName()
  {
   
    return m_conf;
  }

  ProcessorBase* ProcessorFileReader::getProcessor(std::string name /*= ""*/)
  {
    return nullptr;
  }

  void ProcessorFileReader::AddProcessor(ProcessorBase* next, std::string /*= ""*/)
  {
    m_next = next;
  }

  void ProcessorFileReader::ProcessorEvent(event_sp ev)
  {
    ev = m_reader->getEventPtr();
    while (m_reader->NextEvent())
    {
      m_next->ProcessorEvent(std::move(ev));
      ev = m_reader->getEventPtr();
    }
  
    m_next->ProcessorEvent(std::move(ev));
  }

  void ProcessorFileReader::print(std::ostream& os)
  {
    std::cout << m_conf << std::endl;
  }



}
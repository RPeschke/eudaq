#include "eudaq/ProcessorBase.hh"
#include "eudaq/FileWriter.hh"
#include <iostream>

namespace eudaq{

  class  ProcessorFileWriter :public ProcessorBase{
  public:
    ProcessorFileWriter(Parameter_ref conf);
    virtual void init() override;
    virtual void ProcessorEvent(event_sp ev) override;
    virtual void end()override;



    virtual ProcessorBase* getProcessor(std::string name = "") override;

    virtual void AddProcessor(ProcessorBase* next, std::string = "") override;

    virtual std::string getName() override;
    virtual void print(std::ostream& os);

  private:
    std::unique_ptr<FileWriter> m_write;
    bool m_first = true;
    ProcessorBase* m_next=nullptr;
    unsigned m_run;
  };
  RegisterProcessor(ProcessorFileWriter, "ProcessorFileWriter");
  ProcessorFileWriter::ProcessorFileWriter(Parameter_ref conf) :ProcessorBase(conf)
  {
    m_write = FileWriterFactory::Create("native", ".raw");
  }

  void ProcessorFileWriter::init()
  {
    std::cout << "   void ProcessorFileWriter::init() " << std::endl;
    m_first = true;
  }

  void ProcessorFileWriter::ProcessorEvent(event_sp ev)
  {
    if (m_first)
    {
      m_first = false;
      m_run = ev->GetRunNumber();
      m_write->StartRun(m_run);
    }

    m_write->WriteBaseEvent(*ev);
if (m_next)
{
    m_next->ProcessorEvent(ev);
}

  }

  void ProcessorFileWriter::print(std::ostream& os)
  {
    os << "ProcessorFileWriter" << std::endl;
  }

  std::string ProcessorFileWriter::getName()
  {
    return "ProcessorFileWriter";
  }

  void ProcessorFileWriter::AddProcessor(ProcessorBase* next, std::string /*= ""*/)
  {
    m_next = next;
  }

  ProcessorBase* ProcessorFileWriter::getProcessor(std::string name /*= ""*/)
  {
    return this;
  }

  void ProcessorFileWriter::end()
  {

  }

}
#include "eudaq/FileWriter.hh"
#include <iostream>
#include "eudaq/Processor.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;
  class  ProcessorFileWriter :public Processor{
  public:
    ProcessorFileWriter(Parameter_ref conf);
    virtual void init() override;
    virtual ReturnParam ProcessorEvent(event_sp ev) override;
    virtual void end()override;





    virtual std::string getName() override;
    virtual void print(std::ostream& os);

  private:
    std::unique_ptr<FileWriter> m_write;
    bool m_first = true;
    ProcessorBase* m_next=nullptr;
    unsigned m_run;
  };
  RegisterProcessor(ProcessorFileWriter, ProcessorNames::file_writer());
  ProcessorFileWriter::ProcessorFileWriter(Parameter_ref conf) :Processor(conf)
  {
    m_write = FileWriterFactory::Create("native", ".raw");
  }

  void ProcessorFileWriter::init()
  {
    std::cout << "   void ProcessorFileWriter::init() " << std::endl;
    m_first = true;
  }

  ReturnParam ProcessorFileWriter::ProcessorEvent(event_sp ev)
  {
    if (m_first)
    {
      m_first = false;
      m_run = ev->GetRunNumber();
      m_write->StartRun(m_run);
    }

    m_write->WriteBaseEvent(*ev);


    return ProcessNext(ev);


  }

  void ProcessorFileWriter::print(std::ostream& os)
  {
    os << "ProcessorFileWriter" << std::endl;
  }

  std::string ProcessorFileWriter::getName()
  {
    return "ProcessorFileWriter";
  }

 

  void ProcessorFileWriter::end()
  {

  }

}
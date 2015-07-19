


#include "eudaq/ProcessorFileReader.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;




  ProcessorFileReader::ProcessorFileReader(Parameter_ref conf, const fileName& Fname) :Processor_add2queue(conf), m_fName(Fname)
  {

  }

  void ProcessorFileReader::print(std::ostream& os)
  {
    std::cout << m_conf.getName() << std::endl;
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



  void ProcessorFileReader::initialize()
  {
    m_status = running;
    m_reader = FileReaderFactory::create(m_fName.get());
  }

}
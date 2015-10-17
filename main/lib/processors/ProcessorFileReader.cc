


#include "eudaq/ProcessorFileReader.hh"

namespace eudaq {
using ReturnParam = ProcessorBase::ReturnParam;




ProcessorFileReader::ProcessorFileReader(
  Parameter_ref conf,
  const fileName& Fname,
  ConnectionName_ref con_
  )
  : Processor_add2queue(conf, con_),
  m_fName(Fname) {

}




ReturnParam ProcessorFileReader::add2queue(event_sp& ev) {
  ReturnParam ret = sucess;
  if (m_first) {
    ev = m_reader->getEventPtr();
    m_first = false;
    return sucess;
  }

  if (m_reader->NextEvent()) {
    ev = m_reader->getEventPtr();

    return sucess;

  }



  return stop;
}



void ProcessorFileReader::initialize() {
  m_first = true;
  m_reader = FileReaderFactory::create(m_fName.get());
}

}
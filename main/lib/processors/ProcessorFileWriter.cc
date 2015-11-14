#include "eudaq/FileWriter.hh"
#include <iostream>

#include "eudaq/ProcessorFileWriter.hh"
#include "eudaq/Processors.hh"

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  Processors::processor_up Processors::fileWriter() {
    return __make_unique<ProcessorFileWriter>();
  }
  Processors::processor_up fileWriter(const std::string& name, const std::string param_/*=""*/) {
    return __make_unique<ProcessorFileWriter>(name,param_);
  }

  ProcessorFileWriter::ProcessorFileWriter(const std::string & name, const std::string & params /*= ""*/):Processor_Inspector(Parameter_t("")),m_default(false),m_name(name),m_params(params)  {

  }

  ProcessorFileWriter::ProcessorFileWriter() : Processor_Inspector(Parameter_t("")) ,m_default(true) {

  }



  void ProcessorFileWriter::init() {
    if (m_default) {
      m_write = FileWriterFactory::Create();
    } else {
      m_write = FileWriterFactory::Create(m_name, m_params);
    }
    m_first = true;
  }



  void ProcessorFileWriter::end() {
    m_write.reset();
  }


 



  ReturnParam ProcessorFileWriter::inspectEvent(const Event& ev, ConnectionName_ref con) {

    if (m_first) {
      m_first = false;
      m_write->StartRun(ev.GetRunNumber());
    }

    try {
      m_write->WriteBaseEvent(ev);
    } catch (...) {
      return ProcessorBase::stop;
    }


    return sucess;
  }

}
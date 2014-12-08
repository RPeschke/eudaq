#include "eudaq/FileWriter.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/Exception.hh"
#include "eudaq/factoryDev.hh"


namespace eudaq {

  
  FileWriter::FileWriter() : m_filepattern(FileNamer::default_pattern) {}

  void FileWriter::WriteBaseEvent(const Event& ev)
  {
    auto dev = dynamic_cast<const eudaq::DetectorEvent *>(&ev);
    if (dev){
      WriteEvent(*dev);
    }
    else
    {
      EUDAQ_THROW("unable to convert event into detector Event. this can happen when you try to use the sync algorithm which does not create detector events.");
    }
  }



  std::unique_ptr<FileWriter> FileWriterFactory::Create(const std::string & name, const std::string & params /*= ""*/)
  {
    return EUDAQ_Utilities::Factory<FileWriter>::Create(name, params);
  }

  std::vector<std::string> FileWriterFactory::GetTypes()
  {
    return EUDAQ_Utilities::Factory<FileWriter>::GetTypes();
  }

  registerBaseClassDev(FileWriter);

 
}

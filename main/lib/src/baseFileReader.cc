#include <list>
#include "eudaq/baseFileReader.hh"
#include "eudaq/factoryDev.hh"
#include "eudaq/Exception.hh"

namespace eudaq {





  baseFileReader::baseFileReader(Parameter_ref fileName) :baseFileReader(fileName[0])
  {

  }

  baseFileReader::baseFileReader(const std::string& fileName) : m_fileName(fileName)
  {

  }

  std::string baseFileReader::Filename() const
  {
    return m_fileName;
  }

  void baseFileReader::Interrupt()
  {

  }

  registerBaseClassDev(baseFileReader);

  std::unique_ptr<baseFileReader> Factory_file_reader(const std::string & filename, const std::string & filepattern)
  {
    // return nullptr;
    baseFileReader::Parameter_t m;
    m.push_back(filename);
    m.push_back(filepattern);
    if (filename.find_first_not_of("0123456789") == std::string::npos) {
      // filename is run number. using default file reader
      return EUDAQ_Utilities::Factory<baseFileReader>::Create("raw", m);

    }
    else
    {
      auto index_raute =filename.find_last_of('#');
      if (index_raute==std::string::npos)
      {
        // no explicit type definition 
        auto index_dot = filename.find_last_of('.');
        if (index_dot!=std::string::npos)
        {
          std::string type = filename.substr(index_dot + 1);
          return EUDAQ_Utilities::Factory<baseFileReader>::Create(type, m);


        }
        else{
          EUDAQ_THROW("unknown file type ");
        }


      }
      else
      {
        // Explicit definition
        std::string type = filename.substr(index_raute + 1);
        return EUDAQ_Utilities::Factory<baseFileReader>::Create(type, m);

      }


    }


    return nullptr;
   
  }

}

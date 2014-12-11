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



  std::pair<std::string, std::string> split_name_identifier(const std::string& name){
  
    std::pair<std::string, std::string> ret;
  

    auto index_raute = name.find_last_of('#');
    if (index_raute == std::string::npos)
    {
      // no explicit type definition 
      auto index_dot = name.find_last_of('.');
      if (index_dot != std::string::npos)
      {
        std::string type = name.substr(index_dot + 1);
              

        ret.first = type;
        ret.second = name;
      

      }
      else{
        EUDAQ_THROW("unknown file type ");
      }


    }
    else
    {
      // Explicit definition
      std::string type = name.substr(index_raute + 1);
      std::string name_shorted = name.substr(0, index_raute);
   
      ret.first = type;
      ret.second = name_shorted;

      

    }

    return ret;
  }

  std::unique_ptr<baseFileReader> Factory_file_reader(const std::string & filename, const std::string & filepattern)
  {
    // return nullptr;
    baseFileReader::Parameter_t m;
 
    if (filename.find_first_not_of("0123456789") == std::string::npos) {
      // filename is run number. using default file reader
     
      auto splitted_pattern = split_name_identifier(filepattern);
      m.push_back(filename);
      
      m.push_back(splitted_pattern.second);
      return EUDAQ_Utilities::Factory<baseFileReader>::Create(splitted_pattern.first, m);

    }
    else
    {

      auto splitted_filename = split_name_identifier(filename);
      m.push_back(splitted_filename.second);

      m.push_back(filepattern);
      return EUDAQ_Utilities::Factory<baseFileReader>::Create(splitted_filename.first, m);


    }


    return nullptr;
   
  }

  std::string Help_text_File_reader()
  {
    std::string ret = "\n =========== \n";
    ret += "HELP: File Reader: \n";
    ret += "The file reader has two inputs the \"file name\" and the \"file pattern\" \n";
    ret += "option one:\n";
    ret += "\"file name\" = run number\n";
    ret += "In this case the \"file pattern\" gets expanded to a \"file name\" \n";
    ret += "example: \n";
    ret += "\"file pattern\" = ../data/run$6R.raw \n";
    ret += "\"file name\" = 1\n";
    ret += "this expands to: \n";
    ret += "\"file name\" = ../data/run000001.raw\n";
    ret += "option two:\n";
    ret += "\"file name\" = file path\n";
    ret += "in this case the file pattern gets ignored and and only the \"filename\" is taken.\n \n";
    ret += "The file reader gets chosen by the file extension. \n";
    ret += "one can force the use of a specific \"file reader\" by adding #<extension> \n";
    ret += "example:\n";
    ret += "../data/run000001.raw#raw2\n \n";
    ret += "\"file pattern\" = ../data/run$6R.raw#raw2 \n \n";
    ret += "possible \"file readers\" are " + to_string(EUDAQ_Utilities::Factory<baseFileReader>::GetTypes(), ", ")+"\n";


    return ret;
  }

}

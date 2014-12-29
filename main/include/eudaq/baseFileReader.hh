#ifndef baseFileReader_h__
#define baseFileReader_h__
#include <string>
#include <memory>
#include "Platform.hh"
#include "eudaq/factory.hh"
#define RegisterFileReader(derivedClass,ID) registerClass(baseFileReader,derivedClass,ID)


namespace eudaq{
  class Event;
  class DLLEXPORT baseFileReader{
  public:
    using MainType = std::string;
    using Parameter_t = std::vector<std::string>;
    using Parameter_ref = const Parameter_t&;
    baseFileReader(Parameter_ref fileName);
    baseFileReader(const std::string&  fileName);
    std::string Filename()const;


    virtual unsigned RunNumber() const = 0;
    virtual bool NextEvent(size_t skip = 0) = 0;
    virtual std::shared_ptr<eudaq::Event> GetNextEvent() = 0;
    virtual const eudaq::Event & GetEvent() const = 0;
    virtual void Interrupt();

  private:
    std::string m_fileName;

  };


  std::unique_ptr<baseFileReader> DLLEXPORT Factory_file_reader(const std::string & filename, const std::string & filepattern="");
  
  std::string DLLEXPORT Help_text_File_reader();
}

#endif // baseFileReader_h__

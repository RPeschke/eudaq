#ifndef baseFileReader_h__
#define baseFileReader_h__
#include <string>
#include <memory>
#include "Platform.hh"
#include "eudaq/factory.hh"


#define RegisterFileReader(derivedClass,ID) registerClass(eudaq::baseFileReader,derivedClass,ID)


namespace eudaq{
  class Event;
  class OptionParser;
  class DLLEXPORT baseFileReader{
  public:
    enum parameterName{
      File_name ,
      Input_pattern,
      first_users_index
    };
    using MainType = std::string;
    using Parameter_t = std::vector<std::string>;
    using Parameter_ref = const Parameter_t&;
    baseFileReader(Parameter_ref fileName);
    baseFileReader(const std::string&  fileName);
    std::string Filename()const;
    std::string InputPattern() const;
    std::string UserParameter(size_t ID)const;
    size_t userParameterSize() const;
    virtual unsigned RunNumber() const = 0;
    virtual bool NextEvent(size_t skip = 0) = 0;
    virtual std::shared_ptr<eudaq::Event> GetNextEvent() = 0;
    virtual const eudaq::Event & GetEvent() const = 0;
    virtual std::shared_ptr<eudaq::Event> getEventPtr()  = 0 ;
    virtual void Interrupt();

  private:
    Parameter_t m_fileName;
    

  };



  class DLLEXPORT FileReaderFactory{
  public:
    
    static std::unique_ptr<baseFileReader> create(const std::string & filename, const std::string & filepattern);
    static std::unique_ptr<baseFileReader> create(const std::string & filename);

    static std::unique_ptr<baseFileReader> create(eudaq::OptionParser & op);


    static void addComandLineOptions(eudaq::OptionParser & op);
    static std::string Help_text();
    static std::string getDefaultInputpattern();

  private:

    class Impl;
    static Impl& getImpl();

  };
}

#endif // baseFileReader_h__

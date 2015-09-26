#ifndef Processor_parallel_file_reader_h__
#define Processor_parallel_file_reader_h__
#include "eudaq/Processor_Parrallel_add2queue.hh"
#include "eudaq/types/fileName.hh"


namespace eudaq{

  class DLLEXPORT Processor_parallel_file :public Processor_Parrallel_add2queue{

  public:
    
    Processor_parallel_file(Parameter_ref conf,const fileName& fName);

    virtual ~Processor_parallel_file(){}
	virtual void initialize() override;
  virtual void finish() override;

    virtual ReturnParam ProcessEvent(event_sp ev);
    virtual void print(std::ostream& os);
    void setFileName(const std::string& filename);
  private:
    Processor_up m_reader = nullptr;
    fileName m_fileName;
  };


  class parallel_fileReader{
  public:
    parallel_fileReader(const std::string& name) :m_name(name){}

    std::unique_ptr<Processor_parallel_file> FileName(const std::string& filename){
      auto ret = __make_unique<Processor_parallel_file>(ProcessorConf(m_name), fileName(filename));
    
      return ret;
    }
    const std::string& m_name;
  };


}

#endif // Processor_parallel_file_reader_h__

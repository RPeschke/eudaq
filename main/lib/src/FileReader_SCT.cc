#include <list>
#include "eudaq/baseFileReader.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/Event.hh"
#include "eudaq/Logger.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/factory.hh"
#include "eudaq/SCT_defs.hh"

const std::string EventID = "SCT";
const unsigned UnsetVariable = (unsigned) -1;
namespace eudaq {

 
  std::string sct::SCT_FileReader_Name(){
    return "sct";
  }
  using namespace sct;
  FileReader_up Create_SCT_Filereader(const std::string& fileName){

    return FileReaderFactory::create(SCT_FileReader_Name(), baseFileReader::getConfiguration(fileName, ""));

  }

  class DLLEXPORT FileReaderSCT : public baseFileReader {
  public:
    using event_p = std::shared_ptr < eudaq::Event > ;
    using rawEvent_p = std::shared_ptr < eudaq::RawDataEvent > ;
    using detEvent_p = std::shared_ptr < eudaq::DetectorEvent > ;
    FileReaderSCT(const std::string & filename, const std::string & filepattern = "");

    FileReaderSCT(Parameter_ref param);


    ~FileReaderSCT();
    virtual  unsigned RunNumber() const;
    virtual bool NextEvent(size_t skip = 0);
    virtual std::shared_ptr<eudaq::Event> getEventPtr() { return m_ev; }
    virtual std::shared_ptr<eudaq::Event> GetNextEvent();
    virtual void Interrupt() { }



    const eudaq::Event & GetEvent() const;
    const DetectorEvent & Event() const { return GetDetectorEvent(); } // for backward compatibility
    const DetectorEvent & GetDetectorEvent() const;
    const StandardEvent & GetStandardEvent() const;
    std::shared_ptr<eudaq::DetectorEvent> GetDetectorEvent_ptr(){ return std::dynamic_pointer_cast<eudaq::DetectorEvent>(m_ev); };

  private:

    void process_event(){
      event_process_ODD_line();
      event_process_even_lines();

    }
    void event_process_ODD_line();
    void event_process_even_lines();
    void process_trigger(){
      Trigger_process_TDC();
      Trigger_process_TLU();
      Trigger_process_Timestamps();
    }
    void Trigger_process_TDC();
    void Trigger_process_TLU();
    void Trigger_process_Timestamps();
    void checkForEventNumberMissmatch();
    detEvent_p m_ev;
    rawEvent_p m_raw_ev;
    unsigned m_ver, m_eventNumber = 0, m_runNum = 0;
    size_t m_subevent_counter = 0;
    std::string m_triggerFileName;
    std::unique_ptr<std::ifstream> m_eventFile, m_triggerFile;

  };




  FileReaderSCT::FileReaderSCT(const std::string & file, const std::string & filepattern)
    : baseFileReader(FileNamer(filepattern).Set('X', ".raw").SetReplace('R', file)),
    m_ver(1)
  {
    event_p dummy = std::make_shared<eudaq::RawDataEvent>(RawDataEvent::BORE(EventID, 0));

    m_ev = std::make_shared<eudaq::DetectorEvent>(0, 0, 0);
    m_ev->AddEvent(dummy);

    auto name = baseFileReader::Filename();

    auto splittedName = split(name, "_.");

    auto filePathIndex = name.find_last_of("\\/");
    auto filepath = name.substr(0, filePathIndex);
    std::string triggername("TriggerData_");
    m_triggerFileName = filepath + "/" + triggername + splittedName.at(splittedName.size() - 3) + "_" + splittedName.at(splittedName.size() - 2) + ".dat";
    m_eventFile = std::unique_ptr<std::ifstream>(new std::ifstream(name));
    m_triggerFile = std::unique_ptr<std::ifstream>(new std::ifstream(m_triggerFileName));
    
//     std::string line;
//     std::getline(*m_triggerFile, line);
  }

  FileReaderSCT::FileReaderSCT(Parameter_ref param) :FileReaderSCT(param.Get(getKeyFileName(), ""), param.Get(getKeyFileName(),""))
  {
    std::cout << param.Get("offset",(int)-1) << std::endl;
  }

  FileReaderSCT::~FileReaderSCT() {

  }

  bool FileReaderSCT::NextEvent(size_t skip) {



    m_raw_ev = std::make_shared<RawDataEvent>(EventID, 0, m_eventNumber);

    bool result = true;// m_des.ReadEvent(m_ver, ev, skip);



    if (
      !m_eventFile->eof()
      &&
      !m_triggerFile->eof()
      )
    {
      try{
        process_event();
        process_trigger();

        m_ev = std::make_shared<DetectorEvent>(m_runNum, m_eventNumber++, m_raw_ev->GetTimestamp());

        m_ev->AddEvent(std::dynamic_pointer_cast<eudaq::Event>(m_raw_ev));


      }
      catch (...){
#ifdef _DEBUG
        std::cout << "unexpected end of file " << std::endl;
#endif //_DEBUG
        m_ev = nullptr;
        return false;
      }
#ifdef _DEBUG
   //   checkForEventNumberMissmatch();
#endif
      return true;
    }
    else{
      m_ev = nullptr;
      
#ifdef _DEBUG
      std::cout << "end of file" << std::endl;
#endif //_DEBUG
      return false;
    }
    
  }

  unsigned FileReaderSCT::RunNumber() const {
    return m_ev->GetRunNumber();
  }

  const Event & FileReaderSCT::GetEvent() const {
    return *m_ev;
  }

  const DetectorEvent & FileReaderSCT::GetDetectorEvent() const {
    return dynamic_cast<const DetectorEvent &>(*m_ev);
  }

  const StandardEvent & FileReaderSCT::GetStandardEvent() const {
    return dynamic_cast<const StandardEvent &>(*m_ev);
  }

  std::shared_ptr<eudaq::Event> FileReaderSCT::GetNextEvent(){

    if (!NextEvent()) {
      return nullptr;
    }

    return m_ev;


  }

  void FileReaderSCT::event_process_ODD_line()
  {
    std::string line;
    std::getline(*m_eventFile, line);
    if (line.empty())
    {
      EUDAQ_THROW("unexpected end of file");
    }
    auto splittedline = split(line, " ");
    m_raw_ev->SetTag(Event_L0ID(), to_hex(hex2uint_64(splittedline.at(5)), 4));
    m_raw_ev->SetTag(Event_BCID(), to_hex(hex2uint_64(splittedline.at(7)), 4));
  }

  void FileReaderSCT::event_process_even_lines()
  {
    std::string line;
    std::getline(*m_eventFile, line);

    line.erase(0, line.find_first_not_of(" "));
    line.erase(line.find_last_not_of(" ") + 1);

    if (line.empty())
    {
      EUDAQ_THROW("unexpected end of file");
    }

    size_t size_of_bool_data = line.size();
    bool* bool_data = new bool[size_of_bool_data];
    size_t i = 0;
    for (auto& e : line)
    {
      if (e == '1')
      {
        bool_data[i] = true;
      }
      else if (e == '0')
      {
        bool_data[i] = false;
      }
      ++i;
    }

    std::vector<unsigned char> char_data;
    bool2uchar(bool_data, bool_data + size_of_bool_data, char_data);
    delete[] bool_data;
    
    m_raw_ev->AddBlock(0,char_data);
  }

  void FileReaderSCT::Trigger_process_TDC()
  {
    std::string line;
    std::getline(*m_triggerFile, line);
    if (line.empty())
    {
      EUDAQ_THROW("unexpected end of file");
    }

    if (line.find("Trigger data packet") != std::string::npos )
    {
      Trigger_process_TDC();
      return;
    }
    auto splittedline = split(line, " ");
    m_raw_ev->SetTag(TDC_data(), to_hex(hex2uint_64(*(splittedline.end() - 1)),5) );
    m_raw_ev->SetTag(TDC_L0ID(), to_hex(hex2uint_64(*(splittedline.end() - 2)),4) );

  }

  void FileReaderSCT::Trigger_process_TLU()
  {
    std::string line;
    std::getline(*m_triggerFile, line);
    if (line.empty())
    {
      EUDAQ_THROW("unexpected end of file");
    }
    auto splittedline = split(line, " ");
    m_raw_ev->SetTag(TLU_TLUID(), to_hex(hex2uint_64(*(splittedline.end() - 1)) ,4));
    m_raw_ev->SetTag(TLU_L0ID(), to_hex(hex2uint_64(*(splittedline.end() - 3)),4) );
  }

  void FileReaderSCT::Trigger_process_Timestamps()
  {
    std::string line;
    std::getline(*m_triggerFile, line);
    if (line.empty())
    {
      EUDAQ_THROW("unexpected end of file");
    }
    auto splittedline = split(line, " ");

    m_raw_ev->SetTag(Timestamp_data(), to_hex(hex2uint_64(*(splittedline.end() - 1)),10));
    m_raw_ev->SetTag(Timestamp_L0ID(), to_hex(hex2uint_64(*(splittedline.end() - 2)),4));
    m_raw_ev->setTimeStamp(hex2uint_64(splittedline.back()));
  }

  void FileReaderSCT::checkForEventNumberMissmatch()
  {
    if (m_raw_ev->GetTag("tdc_ev_num", UnsetVariable) != m_raw_ev->GetEventNumber()
        ||
        m_raw_ev->GetTag("TLU_ev_num", UnsetVariable) != m_raw_ev->GetEventNumber()
        ||
        m_raw_ev->GetTag("TS_ev_num", UnsetVariable) != m_raw_ev->GetEventNumber()
        )
    {
      EUDAQ_THROW("event number missmatch");

    }
  }





  RegisterFileReader(FileReaderSCT, SCT_FileReader_Name());

}

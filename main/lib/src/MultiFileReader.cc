#include "eudaq/MultiFileReader.hh"
#include "eudaq/AidaFileReader.hh"
#include "eudaq/FileReader.hh"
#include "eudaq/baseFileReader.hh"

#include "eudaq/Event.hh"
#include "eudaq/DetectorEvent.hh"
void eudaq::multiFileReader::addFileReader( const std::string & filename, const std::string & filepattern /*= ""*/ )
{
  m_fileReaders.push_back(Factory_file_reader(filename, filepattern));
  
  auto ev = m_fileReaders.back()->GetNextEvent();
  
  while (ev->IsBORE())
  {

    if (!m_ev)
    {
      
      m_ev = std::make_shared<DetectorEvent>(ev->GetRunNumber(),0,ev->GetTimestamp());
    }
    m_sync->addBORE_Event(m_fileReaders.size() - 1, *ev);
    std::dynamic_pointer_cast<DetectorEvent>(m_ev)->AddEvent(ev);
    
    ev = m_fileReaders.back()->GetNextEvent();
  
  } 
 
}

void eudaq::multiFileReader::Interrupt()
{
  for (auto& p:m_fileReaders)
  {p->Interrupt();
  }
}

bool eudaq::multiFileReader::NextEvent( size_t skip /*= 0*/ )
{
  if (!m_preaparedForEvents)
  {
    m_sync->PrepareForEvents();
    m_preaparedForEvents=true;
  }
  for (size_t skipIndex=0;skipIndex<=skip;skipIndex++)
  {
  
  do{
    for (size_t fileID = 0; fileID < m_fileReaders.size(); ++fileID)
    {

      auto ev = m_fileReaders[fileID]->GetNextEvent();

      if (ev==nullptr&&m_sync->SubEventQueueIsEmpty(fileID))
      {
        return false;
      }
      m_sync->AddEventToProducerQueue(fileID,ev);
    }
    //m_sync.storeCurrentOrder();
  }while (!m_sync->SyncNEvents(m_eventsToSync));
  
  if (!m_sync->getNextEvent(m_ev))
  {
    return false;
  }
  
  }	
  return true;
}

const eudaq::DetectorEvent & eudaq::multiFileReader::GetDetectorEvent() const
{
      return dynamic_cast<const eudaq::DetectorEvent &>(*m_ev);
}

const eudaq::Event & eudaq::multiFileReader::GetEvent() const
{
    return *m_ev;
}

eudaq::multiFileReader::multiFileReader(bool sync) : m_eventsToSync(0), m_preaparedForEvents(0)
{
 // m_sync = factory_sync_class("aida", sync);
  
}

unsigned eudaq::multiFileReader::RunNumber() const
{ 
  return m_ev->GetRunNumber();
}

void eudaq::multiFileReader::addSyncAlgorithm(std::unique_ptr<SyncBase> sync)
{
  if (m_sync)
  {
    EUDAQ_THROW("Sync algoritm already defined");
  }
  m_sync = std::move(sync);
}

void eudaq::multiFileReader::addSyncAlgorithm(SyncBase::MainType type /*= ""*/, SyncBase::Parameter_ref sync/*= 0*/)
{
  addSyncAlgorithm(factory_sync_class(type, sync));
}

eudaq::multiFileReader::strOption_ptr eudaq::multiFileReader::add_Command_line_option_inputPattern(OptionParser & op)
{
  return eudaq::multiFileReader::strOption_ptr(new eudaq::Option<std::string>(op, "i", "inpattern", "../data/run$6R.raw", "string", "Input filename pattern"));

}

std::string eudaq::multiFileReader::help_text()
{
  return Help_text_File_reader();
}


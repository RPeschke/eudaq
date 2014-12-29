#include <list>
#include <iostream>
#include "jsoncons/json.hpp"
#include "eudaq/JSONimpl.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/AidaPacket.hh"
#include "eudaq/Logger.hh"
#include "eudaq/FileSerializer.hh"
#include "eudaq/AidaFileReader.hh"
#include "eudaq/PluginManager.hh"
#include <memory>


namespace eudaq {

	AidaFileReader::AidaFileReader(const std::string & file) :baseFileReader(file),
	  m_runNumber( -1 )
  {
	  m_des = new FileDeserializer(Filename() );
	  m_des->read( m_json_config );
	  readNext();
  }

  AidaFileReader::AidaFileReader(Parameter_ref filename) :AidaFileReader(FileNamer(filename[1]).Set('X', ".raw2").SetReplace('R', filename[0]))
  {

  }

  AidaFileReader::~AidaFileReader() {
	  if ( m_des )
		  delete m_des;
  }

  bool AidaFileReader::readNext() {
	  if ( !m_des || !m_des->HasData() )
		  return false;
	  m_packet = PacketFactory::Create( *m_des );
	  return true;
  }

  std::string AidaFileReader::getJsonPacketInfo() {
	  if ( !m_packet )
		  return "";

	  auto json = m_packet->toJson( AidaPacket::JSON_HEADER | AidaPacket::JSON_METADATA | AidaPacket::JSON_DATA );
	  return json->to_string();
  }


  std::vector<uint64_t> AidaFileReader::getData() {
	  return m_packet->GetData();
  }

  std::shared_ptr<eudaq::Event> AidaFileReader::GetNextEvent() 
  {
	 
	  auto evPack = std::dynamic_pointer_cast<EventPacket>(m_packet);
	  if (evPack!=nullptr)
	  {
		  return GetNextEventFromEventPacket(evPack);
	  }
	  
    m_ev = GetNextEventFromPacket();
	  return m_ev;
	

  }

  std::shared_ptr<eudaq::Event> AidaFileReader::GetNextEventFromPacket()
  {
	  static size_t itter = 0;
	  if (itter < PluginManager::GetNumberOfROF(*m_packet))
	  {
		  return PluginManager::ExtractEventN(m_packet, itter++);
	  }
	  else
	  {

		  itter = 0;

		  if (readNext())
		  {
			  return GetNextEvent();
		  }
	  }

	  return nullptr;
  }

  std::shared_ptr<eudaq::Event> AidaFileReader::GetNextEventFromEventPacket(std::shared_ptr<EventPacket>& eventPack)
  {
	 
	  if (eventPack->getEventPointer()->IsPacket())
	  {
      if (itter<PluginManager::GetNumberOfROF(*eventPack->getEventPointer()))
      {
        return PluginManager::ExtractEventN(eventPack->getEventPointer(),itter++);
      }
      else
      {
        itter = 0;
        if (readNext())
        {
          return GetNextEvent();
        }
      }

    }
    else
    {
      return eventPack->getEventPointer();
    }
	  return nullptr;
  }

  bool AidaFileReader::NextEvent(size_t skip /*= 0*/)
  {
    for (size_t i = 0; i < skip;++i)
    {
      if (!readNext())
      {
        return false;
      }
    }
    return true;
  }

  const eudaq::Event & AidaFileReader::GetEvent() const
  {
    return *m_ev;
  }


    


  RegisterFileReader(AidaFileReader, "raw2");
}

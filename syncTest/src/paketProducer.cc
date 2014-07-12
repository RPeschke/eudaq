#include "paketProducer.h"
#include "eudaq\AidaPacket.hh"


namespace eudaq{
  void paketProducer::startDataTaking( int readoutSpeed )
  {
    m_readoutSpeed=readoutSpeed;
    m_thread=std::unique_ptr<std::thread>(new std::thread(&paketProducer::run,this));
  }

  void paketProducer::addDummyData( uint64_t* meta_data, size_t meta_data_size, uint64_t* data, size_t data_size )
  {
    m_data.emplace_back(meta_data,meta_data_size,data,data_size);
  }

  void paketProducer::run()
  {
    while (m_running)
    {


      int i=0;
      for(auto &e:m_data){
        auto packet = std::make_shared<AidaPacket>(AidaPacket::str2type( m_paketName ), i++ );
        for ( int i = 0; i < e.m_meta_data_size; i++ )
          packet->GetMetaData().v.push_back( e.m_meta_data[i] );
        packet->SetData( e.m_data, e.m_data_size );
        m_dataqueue->pushPacket(packet);
      
      }

      Sleep(m_readoutSpeed);
    }
  }

  void paketProducer::addDataQueue( dataQueue* dqueue )
  {
    if (!m_dataqueue)
    {
      m_dataqueue=dqueue;
    }else{
      std::cout<< " data queue already defined " <<std::endl;
    }

  }

}
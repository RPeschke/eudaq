#ifndef paketProducer_h__
#define paketProducer_h__
#include <thread>
#include <vector>
#include <memory>

namespace eudaq{
  class dataQueue;


  class paketProducer{
  public:
    paketProducer(const char* name):m_paketName(name){}
    void addDataQueue(dataQueue* dqueue);
    void addDummyData( uint64_t* meta_data, size_t meta_data_size, uint64_t* data, size_t data_size );
    void startDataTaking(int readoutSpeed);
    void stopDataTaking();
    void run();
  private:
    std::string m_paketName;
    int m_readoutSpeed;
    dataQueue* m_dataqueue;
    std::unique_ptr<std::thread> m_thread;
    struct data_struc
    {
      uint64_t* m_meta_data;
      size_t m_meta_data_size;
      uint64_t* m_data;
      size_t m_data_size;
    };
    std::vector<data_struc> m_data;
  };

  void paketProducer::addDummyData( uint64_t* meta_data, size_t meta_data_size, uint64_t* data, size_t data_size )
  {
    m_data.emplace_back(meta_data,meta_data_size,data,data_size);
  }

  void paketProducer::startDataTaking( int readoutSpeed )
  {
    m_readoutSpeed=readoutSpeed;
    m_thread=std::unique_ptr<std::thread>(new std::thread(&run,this));

  }

}
#endif // paketProducer_h__

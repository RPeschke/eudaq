#ifndef paketProducer_h__
#define paketProducer_h__
#include <thread>
#include <vector>
#include <memory>
#include "dataQueue.h"

namespace eudaq{
  class dataQueue;


  class paketProducer{
  public:
    paketProducer(const char* name, int type) :
      m_paketName(name),
      m_running(true), 
      m_dataqueue(nullptr),
      m_type(type)
    {
    }
    ~paketProducer(){
      m_running = false;
      if (m_thread && m_thread->joinable())
      {
        m_thread->join();
      }
    }

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
      data_struc(uint64_t* meta_data,size_t meta_data_size,uint64_t* data,size_t data_size):
        m_meta_data(meta_data),
        m_meta_data_size(meta_data_size),
        m_data(data),
        m_data_size(data_size)
      {
      }

      uint64_t* m_meta_data;
      size_t m_meta_data_size;
      uint64_t* m_data;
      size_t m_data_size;
    };
    std::vector<data_struc> m_data;
    bool m_running;
    int m_type;
  };

}
#endif // paketProducer_h__

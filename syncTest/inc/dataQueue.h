#ifndef dataQueue_h__
#define dataQueue_h__
#include <memory>
#include <queue>
#include <mutex>
#include "eudaq/AidaPacket.hh"

namespace eudaq{
  class AidaFileWriter;
  class dataQueue{
  public:
    void pushPacket(std::shared_ptr<AidaPacket> packet);
    std::shared_ptr<AidaPacket> getPacket();
    void writeToFile(const char* filename);
    void WritePacket( const AidaPacket & packet );
    void writeNextPacket();
    std::queue<std::shared_ptr<AidaPacket>> m_queue;
    std::mutex m_queueMutex;
    std::shared_ptr<AidaFileWriter> m_writer;
  };
}
#endif // dataQueue_h__

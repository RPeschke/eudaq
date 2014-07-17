#include "dataQueue.h"
#include "eudaq\AidaFileWriter.hh"

namespace eudaq {




  void dataQueue::pushPacket( std::shared_ptr<AidaPacket> packet )
  {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    m_queue.push(packet);

  }

  std::shared_ptr<AidaPacket> dataQueue::getPacket()
  {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    auto ret=m_queue.front();
    m_queue.pop();
    return ret;
  }

  void dataQueue::writeToFile( const char* filename )
  {
    m_writer =  std::shared_ptr<eudaq::AidaFileWriter>(AidaFileWriterFactory::Create("" ));
    m_writer->StartRun(1001);
    m_writer->SetFilePattern("");
  }

  void dataQueue::WritePacket( const AidaPacket & packet )
  {
    if (m_writer.get()) {
      try {
        m_writer->WritePacket( packet );
      } catch(const Exception & e) {
        std::string msg = "Exception writing to file: ";
        msg += e.what();

      }

    }
  }


  void dataQueue::writeNextPacket()
  { auto packet=getPacket();
    WritePacket(*packet);
  }


}
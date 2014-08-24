#include "dataQueue.h"
#include "paketProducer.h"
#include "eudaq/OptionParser.hh"
#include "eudaq/genericDetectorContainer.hh"
#include "eudaq/genericSynchronisation.hh"
#include "eudaq/PluginManager.hh"
#include "eudaq/AidaFileWriter.hh"

using namespace eudaq;
using namespace std;
bool isTLUContainer(shared_ptr<AidaPacket> tlu){
  return tlu->GetPacketSubType() == 3;

}
int main(){

  eudaq::OptionParser op("EUDAQ Sync test", "1.0", "", 1);

  try{


    dataQueue dq;

    shared_ptr<paketProducer> pp = make_shared<paketProducer>("slowP", 1);


    uint64_t meta = 111;
    const char * dataString = "hallo dass ist ein test   ";
    uint64_t * data = (uint64_t*) (dataString);
    size_t size = 3;
    pp->addDummyData(&meta, 1, data, size);
    dq.addNewProducer(pp);
    pp->addDataQueue(&dq);


    auto fastP = make_shared<paketProducer>("fastP", 2);
    uint64_t meta1 = 222;
    const char * dataString1 = "hallo dass ist ein test2  ";
    uint64_t * data1 = (uint64_t*) (dataString1);
    size_t size1 = 3;
    fastP->addDummyData(&meta1, 1, data1, size1);
    dq.addNewProducer(fastP);
    fastP->addDataQueue(&dq);

    auto multiP = make_shared<paketProducer>("multiFrame", 3);
    uint64_t meta2 = 333;
    const char * dataString2 = "hallo dass ist ein test3  ";
    uint64_t * data2 = (uint64_t*) (dataString2);
    size_t size2 = 3;
    multiP->addDummyData(&meta2, 1, data2, size2);
    dq.addNewProducer(multiP);
    multiP->addDataQueue(&dq);



    pp->startDataTaking(100);
    fastP->startDataTaking(0);
    multiP->startDataTaking(20);
    for (int i = 0; i < 50; ++i)
    {
      dq.trigger();
      Sleep(1);
    }

    
    pp->stopDataTaking();
    fastP->stopDataTaking();
    multiP->stopDataTaking();
    GenericPacketSync<eudaq::AidaPacket> sync;
    auto packet1 = std::make_shared<AidaPacket>(AidaPacket::str2type("slowP"), 1);
    auto packet2 = std::make_shared<AidaPacket>(AidaPacket::str2type("fastP"), 2);
    auto packet3 = std::make_shared<AidaPacket>(AidaPacket::str2type("multiFrame"), 3);
    sync.addBOREEvent(packet1);
    sync.addBOREEvent(packet3);
    sync.addBOREEvent(packet2);
    
    sync.PrepareForEvents();

    while (!dq.empty()){
      sync.addElemtToProducerQueue(dq.getPacket());
    }
    Sleep(1);
    do
    {
    } while (sync.SyncFirstEvent());
    Sleep(1);

    auto  m_writer = std::shared_ptr<eudaq::AidaFileWriter>(AidaFileWriterFactory::Create("metaData"));
    m_writer->StartRun(1001);
    m_writer->SetFilePattern("");
    eudaq::GenericPacketSync<AidaPacket>::detPointer ev;

    while (sync.getNextEvent(ev))
    {
      for (int i = 0; i < ev->Size(); ++i)
      {
        m_writer->WritePacket(ev->getElement(i));
      }

    }


  }
  catch (...){
    return op.HandleMainException();

  }

  return 0;
}
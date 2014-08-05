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
  return tlu->GetPacketSubType() == 2;

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
    auto pp1 = make_shared<paketProducer>("fastP", 2);

    uint64_t meta1 = 222;
    const char * dataString1 = "hallo dass ist ein test2  ";
    uint64_t * data1 = (uint64_t*) (dataString1);
    size_t size1 = 3;
    pp1->addDummyData(&meta1, 1, data1, size1);
    dq.addNewProducer(pp1);
    pp1->addDataQueue(&dq);


    pp->startDataTaking(100);
    pp1->startDataTaking(0);
    for (int i = 0; i < 50; ++i)
    {
      dq.trigger();
      Sleep(1);
    }

    
    pp->stopDataTaking();
    pp1->stopDataTaking();

    GenericPacketSync<eudaq::AidaPacket> sync;
    auto packet1 = std::make_shared<AidaPacket>(AidaPacket::str2type("slowP"), 1);
    auto packet2 = std::make_shared<AidaPacket>(AidaPacket::str2type("fastP"), 2);
    sync.addBOREEvent(packet1);
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
    shared_ptr<genericDetContainer<AidaPacket>> ev;
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
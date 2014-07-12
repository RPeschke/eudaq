#include "dataQueue.h"
#include "paketProducer.h"

using namespace eudaq;
using namespace std;
int main(){

  dataQueue dq;

  paketProducer pp("name1");

  pp.addDataQueue(&dq);
  uint64_t meta=111;
  const char * dataString="hallo dass ist ein test   ";
  uint64_t * data=(uint64_t*)(dataString);
  size_t size=3;
  pp.addDummyData(&meta,1,data,size);

  paketProducer pp1("name2");

  pp1.addDataQueue(&dq);
  uint64_t meta1=111;
  const char * dataString1="hallo dass ist ein test2  ";
  uint64_t * data1=(uint64_t*)(dataString1);
  size_t size1=3;
  pp1.addDummyData(&meta1,1,data1,size1);


  pp.startDataTaking(1);
  pp1.startDataTaking(1);
  dq.writeToFile("test.raw");
  while (!dq.m_queue.empty())
  {
    dq.writeNextPacket();
    Sleep(1);
  }

  Sleep(100000);
}
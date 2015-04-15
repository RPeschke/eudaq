#ifndef TSIGNALS_H
#define TSIGNALS_H


#define  _CRT_SECURE_NO_WARNINGS

#include "RQ_OBJECT.h"



class  TSignals {
    RQ_OBJECT("TSignals")
  public:
    TSignals() {}
    void StartOfBurst() { Emit("StartOfBurst()"); }
    void StartEvent(int nev) { Emit("StartEvent(int)", nev); }
    void EventData(int id,int nstrips,bool* data0,bool* data1) {
       EmitVA("EventData(int,int,bool*,bool*)", 4, id, nstrips, data0, data1);
    }
    void EndEvent(int nev) { Emit("EndEvent(int)", nev); }
    void EndOfBurst() { Emit("EndOfBurst()"); }
    Bool_t ConnectCINT(const char *signal,
                       const char *slot) {
      return ::ConnectCINT(&fQObject, signal, slot);
        }
    ClassDef(TSignals, 1);
};

#endif
 
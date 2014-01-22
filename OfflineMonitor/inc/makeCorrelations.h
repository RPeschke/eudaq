

#ifndef makeCorrelations_h__
#define makeCorrelations_h__
#include <vector>
#include "Rtypes.h"
#include "eudaq\DetectorEvent.hh"


class TFile;
class TTree;
class plane;
class TH2D;
class TH1D;
class CorrelationPlot;


class mCorrelations{
public:
	mCorrelations();
	~mCorrelations();
//void open_inFile(const char * InFileName);
void open_confFile(const char * InFileName);
void createHistograms();
void open_outFile(const char * outFileName);
//void Process();
void clear();
bool ProcessDetectorEvent(const eudaq::DetectorEvent &);
void ProcessCurrentEntry();
//bool ProcessNextEvent();
void AddCurrentEntryToCalibration();
void CalibrateIgnore();
void AddCurrentEntryToHistograms();
void fillCorrelations();
//void compare( plane& planeA, plane& planeB);
//bool getNextEvent();



private:

//TFile* inputFile;
TFile* OutPutFile;
//TH2D *h2,*h3;
//TTree* tree;
std::vector<plane> m_planes;
std::vector<CorrelationPlot> m_corr;
Long64_t currentEvent;//,pos,endPos;
Double_t m_hit_x,m_hit_y;
Int_t m_plane_id,m_event_id,m_CalibrationEvents,NumberOfEvents;
};



#endif // makeCorrelations_h__

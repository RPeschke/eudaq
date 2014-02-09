

#ifndef makeCorrelations_h__
#define makeCorrelations_h__
#include <vector>
#include "Rtypes.h"
#include "eudaq\DetectorEvent.hh"
#include "rapidxml_utils.hpp"
#include "CorrelationPlots_interface.h"



class TFile;
class TTree;
class plane;
class TH2D;
class TH1D;
class CorrelationPlot;
class CorrelationVSTimePlots;


class mCorrelations{
public:
	mCorrelations();
	~mCorrelations();
//void open_inFile(const char * InFileName);
void open_confFile(const char * InFileName);
void register_planes(rapidxml::xml_node<> *planesNode);
void register_Correlations(rapidxml::xml_node<> *correlationsNode);
void register_CorrelationsVsTime(rapidxml::xml_node<> *correlationsTimeNode);
void register_planes2Correlation();
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
std::vector<CorrelationPlots_interface*> m_corr;
//std::vector<CorrelationVSTimePlots> m_corrVStime;
Long64_t currentEvent;//,pos,endPos;
Double_t m_hit_x,m_hit_y;
Int_t m_plane_id,m_event_id,m_CalibrationEvents,NumberOfEvents;
};



#endif // makeCorrelations_h__

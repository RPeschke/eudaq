#include "makeCorrelations.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include <iostream>
#include "TSystem.h"
#include <fstream>
#include "TH1.h"
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <sstream>
#include "Planes.h"
#include "CorrelationPlots.h"
#include "..\..\main\include\eudaq\PluginManager.hh"
#include "..\..\main\include\eudaq\StandardEvent.hh"

using namespace std;
using namespace eudaq;





mCorrelations::mCorrelations() :OutPutFile(nullptr),m_CalibrationEvents(0)
{

}


// void mCorrelations::open_inFile( const char * InFileName )
// {
// 	if (inputFile)
// 	{
// 		cout<<"file already open \n";
// 		return;
// 	}
// 	inputFile=new TFile(InFileName);
// 	tree=(TTree*)inputFile->Get("tree");
// 	tree->SetBranchAddress("id_plane",&m_plane_id);
// 	tree->SetBranchAddress("id_x",&m_hit_x);
// 	tree->SetBranchAddress("id_y",&m_hit_y);
// 	tree->SetBranchAddress("i_event",&m_event_id);
// 	endPos=tree->GetEntries();
// 	tree->GetEntry(0);
// 	currentEvent=m_event_id;
// 	
// }

void mCorrelations::open_confFile( const char * InFileName )
{
	rapidxml::file<> f(InFileName);
	rapidxml::xml_document<> doc; 

	doc.parse<0>(f.data());
	NumberOfEvents=stoi(doc.first_node("configuration")->first_node("events")->first_node("numberOfEvents")->first_attribute("No")->value());
	m_CalibrationEvents=stoi(doc.first_node("configuration")->first_node("events")->first_node("CalibrationEvents")->first_attribute("No")->value());
	auto node = doc.first_node("configuration")->first_node("planes")->first_node("plane");
	
	m_planes.emplace_back(node);
	for (node = node->next_sibling("plane");node;node=node->next_sibling("plane"))
	{
		m_planes.emplace_back(node);
	}
	node = doc.first_node("configuration")->first_node("Correlations")->first_node("Correlation");
	m_corr.emplace_back(node);
	for (node = node->next_sibling("Correlation");node;node=node->next_sibling("Correlation"))
	{
		m_corr.emplace_back(node);
	}
	for(auto &corr:m_corr){

		if (!corr.registerPlanes(m_planes))
		{
			std::cout <<"unable to register Plane: "<< corr.m_planeID0 << " or " << corr.m_planeID1 <<std::endl;
			throw("unable to register Plane");
		}
		
	}

}

void mCorrelations::open_outFile( const char * outFileName )
{
	if (OutPutFile)
	{
		cout<<"file already open \n";
		return;
	}
	OutPutFile=new TFile(outFileName,"RECREATE");
// 	h2=new TH2D("X_Cor","correlations x vs x",2000,0,2000,2000,0,2000);
// 	h3=new TH2D("Y_Cor","correlations y vs y",1000,0,1000,1000,0,1000);
// 	out=new ofstream("test.txt");
// 
// 
// 	for (auto& p:m_planes)
// 	{
// 		p.createHistograms();
// 	}
}

// void mCorrelations::Process()
// {
// 	while (m_event_id<m_CalibrationEvents)
// 	{
// 		AddCurrentEntryToCalibration();
// 		tree->GetEntry(++pos);
// 	}
// 	for (auto& p:m_planes)
// 	{
// 		p.HotPixelsuppression();
// 	}
// 	
// 	currentEvent=m_event_id;
// 	int eventsProcessed=0;
// 	while (ProcessNextEvent()&&++eventsProcessed<NumberOfEvents)
// 	{
// 		for (auto &c:m_corr)
// 		{
// 			c.processEvent();
// 		}
// 	//	compare(m_planes.at(0),m_planes.at(1));
// 	}
// 	cout<<"Events Processed: "<<eventsProcessed<<endl;
// }

bool mCorrelations::ProcessDetectorEvent( const eudaq::DetectorEvent & ev)
{
	if (ev.IsBORE()) {
		eudaq::PluginManager::Initialize(ev);
		return true;
	} else if (ev.IsEORE()) {

		return false;
	}
	clear();
	StandardEvent sev = eudaq::PluginManager::ConvertToStandard(ev);
	for (size_t iplane = 0; iplane < sev.NumPlanes(); ++iplane) {

		const eudaq::StandardPlane & plane = sev.GetPlane(iplane);
		std::vector<double> cds = plane.GetPixels<double>();

		for (size_t ipix = 0; ipix < cds.size(); ++ipix) {

			

			m_plane_id = plane.ID();          
			
			m_hit_x = plane.GetX(ipix);
			m_hit_y= plane.GetY(ipix);
			
			m_event_id = sev.GetEventNumber();                  
			if (m_event_id>NumberOfEvents)
			{
				return false;
			}
			ProcessCurrentEntry();

		}               
	}
	if (m_event_id==m_CalibrationEvents)
	{
		CalibrateIgnore();
	}
	fillCorrelations();
	return true;
}








void mCorrelations::ProcessCurrentEntry()
{

	if (m_event_id>m_CalibrationEvents)
	{
		AddCurrentEntryToHistograms();
	}
	else if (m_event_id<m_CalibrationEvents)
	{
		AddCurrentEntryToCalibration();
	}



}

void mCorrelations::fillCorrelations()
{
	if (m_event_id>m_CalibrationEvents)
	{
		for (auto &c:m_corr)
		{
			c.processEvent();
		}
	}

}

void mCorrelations::AddCurrentEntryToHistograms()
{
	for (auto& p:m_planes)
	{
		p.addCurrentEntry(m_plane_id,m_hit_x,m_hit_y);
	}
}

// bool mCorrelations::ProcessNextEvent()
// {
// 	for (auto& p:m_planes)
// 	{
// 		p.clear();
// 	}
// 	while(currentEvent==m_event_id){
// 		for (auto& p:m_planes)
// 		{
// 			p.addCurrentEntry(m_plane_id,m_hit_x,m_hit_y);
// 		}
// 		if (++pos==endPos)
// 		{
// 			return false;
// 		}
// 		tree->GetEntry(pos);
// 
// 		
// 	}
// 	if (currentEvent%1000==0)
// 	{
// 		std::cout<<currentEvent<<std::endl;
// 	}
// 	currentEvent=m_event_id;
// 	return true;
// }

void mCorrelations::AddCurrentEntryToCalibration()
{
	for (auto& p:m_planes)
	{
		p.addCurrentEntry2CalibrationData(m_plane_id,m_hit_x,m_hit_y);
	}
}



// void mCorrelations::compare(  plane& planeA, plane& planeB )
// {
// 	if (!planeB.empty()&&!planeA.empty())
// 	{
// 
// 	
// 	planeB.firstEntry();
// 	planeA.firstEntry();
// 	while (planeA.nextEntry())
// 	{
// 		while (planeB.nextEntry())
// 		{
// 			h2->Fill(planeA.getX(),planeB.getX());
// 			h3->Fill(planeA.getY(),planeB.getY());
// 		//	*out<<planeA.getX()<< " ; "<<planeB.getX()<<endl;
// 		}
// 		planeB.firstEntry();
// 	}
// 	}
// }

mCorrelations::~mCorrelations()
{
	if (OutPutFile)
	{
//	h2->Write();


	OutPutFile->Write();
	OutPutFile->Close();
	delete OutPutFile;
	}

}

void mCorrelations::createHistograms()
{
	for (auto& p:m_planes)
	{
		p.createHistograms();
	}

	for (auto& c:m_corr)
	{
		c.createHistogram();
	}
}

void mCorrelations::CalibrateIgnore()
{
	for (auto& p:m_planes)
	{
		p.HotPixelsuppression();
	}
}

void mCorrelations::clear()
{
	for (auto& p:m_planes)
	{
		p.clear();
	}
}

bool ignorRegin( double Value,const std::vector<double> & beginVec,const std::vector<double> & endVec )
{
	for(int i=0; i<beginVec.size();++i){

		if (beginVec.at(i)<=Value
			&&
			endVec.at(i)>=Value)
		{
			return true;
		}

	}
	return false;
}



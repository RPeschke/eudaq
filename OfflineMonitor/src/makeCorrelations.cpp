#include "makeCorrelations.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include <iostream>
#include "TSystem.h"
#include <fstream>
#include "TH1.h"
#include "TCanvas.h"
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <sstream>
#include "Planes.h"
#include "CorrelationPlots.h"
#include "CorrelationVSTimePlots.h"
#include "..\..\main\include\eudaq\PluginManager.hh"
#include "..\..\main\include\eudaq\StandardEvent.hh"
#include "eudaq\FileNamer.hh"

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
	
	auto node = doc.first_node("configuration")->first_node("planes");
	register_planes(node);
	
	node = doc.first_node("configuration")->first_node("Correlations");
	register_Correlations(node);


	node = doc.first_node("configuration")->first_node("Correlations");
	register_CorrelationsVsTime(node);

	register_planes2Correlation();
}

void mCorrelations::open_outFile( const char * outFileName )
{
	if (OutPutFile)
	{
		cout<<"file already open \n";
		return;
	}
	std::cout<<" creating output file: "<<outFileName<<std::endl;
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

void mCorrelations::open_outFile()
{
	std::string foutput(FileNamer(m_filepattern).Set('X', ".root").Set('R', m_runNumber));
	open_outFile(foutput.c_str());
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
		m_event_id=0;
		return true;
	} else if (ev.IsEORE()) {

		return false;
	}
	++m_event_id;
	clear();
	StandardEvent sev = eudaq::PluginManager::ConvertToStandard(ev);
	for (size_t iplane = 0; iplane < sev.NumPlanes(); ++iplane) {

		const eudaq::StandardPlane & plane = sev.GetPlane(iplane);
		std::vector<double> cds = plane.GetPixels<double>();

		for (size_t ipix = 0; ipix < cds.size(); ++ipix) {

			

			m_plane_id = plane.ID();          
			
			m_hit_x = plane.GetX(ipix);
			m_hit_y= plane.GetY(ipix);
			
			//m_event_id = sev.GetEventNumber();                  
			if (m_event_id>NumberOfEvents)
			{
				return false;
			}
			if (m_hit_x>0)
			{
				ProcessCurrentEntry();
			}
			

		}               
	}
	if (m_event_id==m_CalibrationEvents)
	{
		CalibrateIgnore();
	}
	if (m_event_id>m_CalibrationEvents)
	{
		fillCorrelations();
	}
	
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

		for (auto &c:m_corr)
		{
			c->processEvent();
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

	while (!m_corr.empty())
	{
		delete m_corr.back();
		m_corr.pop_back();
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
		c->setExpectedEventNumber(NumberOfEvents-m_CalibrationEvents);
		c->createHistogram();
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

void mCorrelations::register_planes( rapidxml::xml_node<> *planesNode )
{
	

	
	for (auto node =planesNode->first_node("plane");node;node=node->next_sibling("plane"))
	{
		m_planes.emplace_back(node);
	}
}

void mCorrelations::register_Correlations( rapidxml::xml_node<> *correlationsNode )
{

	
	for (	auto	node = correlationsNode->first_node("Correlation");node;node=node->next_sibling("Correlation"))
	{
		m_corr.push_back(new CorrelationPlot(node));
	}


}

void mCorrelations::register_CorrelationsVsTime( rapidxml::xml_node<> *correlationsTimeNode )
{
	for (	auto	node = correlationsTimeNode->first_node("CorrelationVSTime");node;node=node->next_sibling("CorrelationVSTime"))
	{
		m_corr.push_back(new CorrelationVSTimePlots(node));
	}



}

void mCorrelations::register_planes2Correlation()
{
	for(auto &corr:m_corr){

		if (!corr->registerPlanes(m_planes))
		{
			std::cout <<"unable to register Plane: "<< corr->m_planeID0 << " or " << corr->m_planeID1 <<std::endl;
			throw("unable to register Plane");
		}

	}
}

void mCorrelations::savePlotsAsPicture()
{
	TCanvas c1;
	int DrawRows=static_cast<int>(m_planes.size()/2)+1+static_cast<int>(m_corr.size()/2)+1;
	std::cout<<DrawRows<<std::endl;
	c1.Divide(2,DrawRows);
	int j=1;
	for (size_t i=0;i<m_planes.size();++i)
	{
		c1.cd(j++);
		m_planes.at(i).Draw();

	}
	for (size_t i=0;i<m_planes.size();++i)
	{
		c1.cd(j++);
		m_corr.at(i)->Draw();

	}
	c1.cd(0);
	std::string foutput(FileNamer(m_filepattern).Set('X', ".pdf").Set('R', m_runNumber));
	c1.SaveAs(foutput.c_str());
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



#include "CorrelationVSTimePlots.h"
#include "CorrelationPlots_interface.h"
#include "TH2D.h"
#include <string>
#include <iostream>

using namespace std;



void CorrelationVSTimePlots::createHistogram()
{
	std::string x_axis_name,y_axis_name;
	if (m_axis0==x_axis)
	{
		m_x_axis=m_plane0->m_x_axis;
		x_axis_name="x";
	}else if(m_axis0==y_axis)
	{
		m_x_axis=m_plane0->m_y_axis;
		x_axis_name="y";
	}

	if (m_axis1==x_axis)
	{
		m_y_axis=m_plane1->m_x_axis;
		y_axis_name="x";
	}else if(m_axis0==y_axis)
	{
		m_y_axis=m_plane1->m_y_axis;
		y_axis_name="y";
	}
	std::string name="correlation_"+to_string(m_planeID0)+"_"+x_axis_name+"_"+to_string(m_planeID1)+"_"+y_axis_name+"_time";
	std::string title="correlation "+to_string(m_planeID0)+" axis "+x_axis_name+" - "+to_string(m_planeID1)+" axis"+y_axis_name +" VS Time";
	m_corr=new TH2D(name.c_str(),title.c_str(),
		1000,0,20000,
		max(m_y_axis.bins,m_x_axis.bins),-500,500);
}

void CorrelationVSTimePlots::processEntry()
{
	double x,y;
	if (m_axis0==x_axis)
	{
		x=m_plane0->getX();
	}else if(m_axis0==y_axis)
	{
		x=m_plane0->getY();
	}

	if (m_axis1==x_axis)
	{
		y=m_plane1->getX();
	}else if(m_axis0==y_axis)
	{
		y=m_plane1->getY();
	}

	if (cutOffCondition(x,y))
	{
		m_corr->Fill(event_nr,x-y);
	}

}

CorrelationVSTimePlots::CorrelationVSTimePlots( rapidxml::xml_node<> *node ) :CorrelationPlots_interface(node),m_corr(nullptr)
{

}

CorrelationVSTimePlots::~CorrelationVSTimePlots()
{
	std::cout<<"CorrelationVSTimePlots::~CorrelationVSTimePlots()"<<endl;
}

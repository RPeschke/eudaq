#include "CorrelationPlots.h"
#include "Planes.h"
#include "TH2.h"
#include <string>
#include <iostream>
#include "XMLextractorHelpers.h"
using namespace std;


// class CorrelationPlot::condition{
// public:
// 	condition(double A,double B,double C ,double D):m_A(A),m_B(B),m_C(C),m_D(D){}
// 
// 	bool isTrue(double x,double y){
// 		return (m_A*x+m_B*y+m_C) < m_D;
// 	}
// private:
// double m_A,m_B,m_C,m_D;
// 
// };

CorrelationPlot::CorrelationPlot( rapidxml::xml_node<> *Correlation ):CorrelationPlots_interface(Correlation),m_corr(nullptr)
{


}




void CorrelationPlot::createHistogram()
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
	std::string name="correlation_"+to_string(m_planeID0)+"_"+x_axis_name+"_"+to_string(m_planeID1)+"_"+y_axis_name;
	std::string title="correlation "+to_string(m_planeID0)+" axis "+x_axis_name+" VS "+to_string(m_planeID1)+" axis"+y_axis_name;
	m_corr=new TH2D(name.c_str(),title.c_str(),
		m_x_axis.bins,m_x_axis.low,m_x_axis.high,
		m_y_axis.bins,m_y_axis.low,m_y_axis.high);
}

void CorrelationPlot::processEntry()
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
		m_corr->Fill(x,y);
	}

}

CorrelationPlot::~CorrelationPlot()
{
	//std::cout<<"CorrelationPlot::~CorrelationPlot()"<<std::endl;
	
}

void CorrelationPlot::Draw( const char* DrawOptions )
{
	m_corr->Draw("colz");
}








#include "CorrelationPlots.h"
#include "Planes.h"
#include "TH2.h"
#include <string>
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

CorrelationPlot::CorrelationPlot( rapidxml::xml_node<> *Correlation )
{
	//auto Correlation=node->first_node("Correlation");
	m_planeID0=std::stoi(Correlation->first_node("CorrX")->first_attribute("planeId")->value());
	m_planeID1=std::stoi(Correlation->first_node("CorrY")->first_attribute("planeId")->value());
	auto axis0=Correlation->first_node("CorrX")->first_attribute("plane_axis")->value();
	if (!strcmp(axis0,"x"))
	{
		m_axis0=x_axis;
	}else if (!strcmp(axis0,"y"))
	{
		m_axis0=y_axis;
	}
	else if (!strcmp(axis0,"z"))
	{
		m_axis0=z_axis;
	}

	auto axis1=Correlation->first_node("CorrY")->first_attribute("plane_axis")->value();
	if (!strcmp(axis1,"x"))
	{
		m_axis1=x_axis;
	}else if (!strcmp(axis1,"y"))
	{
		m_axis1=y_axis;
	}
	else if (!strcmp(axis1,"z"))
	{
		m_axis1=z_axis;
	}

	extractCutOffCondition(Correlation->first_node("cutOffCondition"));

}

bool CorrelationPlot::registerPlanes(std::vector<plane>& planes )
{
	bool register0(0),register1(0);
	for (auto& p:planes)
	{
		if (p.m_plane_id==m_planeID0)
		{
			m_plane0=&p;
			register0=true;
		}
		if (p.m_plane_id==m_planeID1)
		{
			m_plane1=&p;
			register1=true;
		}
	}
	return register0&&register1;
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
	CorrHist=new TH2D(name.c_str(),title.c_str(),
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
		CorrHist->Fill(x,y);
	}
	
}

void CorrelationPlot::processEvent()
{
	if (!m_plane0->empty()&&!m_plane1->empty())
	{


		m_plane0->firstEntry();
		m_plane1->firstEntry();
		do
		{
			do
			{
				processEntry();

				//	*out<<planeA.getX()<< " ; "<<planeB.getX()<<endl;
			}while (m_plane1->nextEntry());
			m_plane1->firstEntry();
		}while (m_plane0->nextEntry());
	}
}

bool CorrelationPlot::cutOffCondition( const double& x,const  double& y ) const
{
	if (m_con.empty())
	{
		return true;
	}
	for (auto& p:m_con)
	{
		if(!p.isTrue(x,y))
		{
			return false;
		}
	}
	return true;
}

void CorrelationPlot::extractCutOffCondition( rapidxml::xml_node<> *cutOffCon )
{
	if (cutOffCon)
	{
		double A,B,C,D;
		auto node =cutOffCon->first_node("condition");
	
	
		while(node)
		{
			A=std::stod(node->first_attribute("A")->value());
			B=std::stod(node->first_attribute("B")->value());
			C=std::stod(node->first_attribute("C")->value());
			D=std::stod(node->first_attribute("D")->value());
			m_con.emplace_back(A,B,C,D);
			node=node->next_sibling("condition");
		}
	}
}



#ifndef CorrelationPlots_h__
#define CorrelationPlots_h__
#include "rapidxml.hpp"
#include <string>
#include <vector>
#include "Planes.h"
class plane;
class TH2D;

enum axis{x_axis,y_axis,z_axis};
class CorrelationPlot{
public:
	CorrelationPlot(rapidxml::xml_node<> *node);

bool registerPlanes(std::vector<plane>& planes);

	void processEntry();
	void processEvent();
	void createHistogram();
	bool cutOffCondition(const double& x, const double& y) const;
	void extractCutOffCondition(rapidxml::xml_node<> *cutOffCon);
	int m_planeID0,m_planeID1;
	axis m_axis0,m_axis1;
	axisProberties m_x_axis,m_y_axis;
	TH2D* CorrHist;
	plane *m_plane0,*m_plane1;
	
	class condition{
	public:
		condition(double A,double B,double C ,double D):m_A(A),m_B(B),m_C(C),m_D(D){}

		bool isTrue(const double& x,const double& y) const{
			return abs(m_A*x+m_B*y+m_C) < m_D;
		}
	private:
		double m_A,m_B,m_C,m_D;

	};
	std::vector<condition> m_con;
};

#endif // CorrelationPlots_h__

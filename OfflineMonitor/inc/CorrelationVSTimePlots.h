#ifndef CorrelationVSTimePlots_h__
#define CorrelationVSTimePlots_h__
#include "rapidxml_utils.hpp"
#include "Planes.h"
#include "Axis.h"
#include "CorrelationPlots_interface.h"

class CorrelationVSTimePlots: public CorrelationPlots_interface{
public:
	CorrelationVSTimePlots(rapidxml::xml_node<> *node);
	~CorrelationVSTimePlots(){}

	virtual void createHistogram();
	virtual void processEntry();
private:
	TH2D* m_corr;
	
};

#endif // CorrelationVSTimePlots_h__

#ifndef EVENTSCALEFACTOR_HH
#define EVENTSCALEFACTOR_HH
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class EventScaleFactor : public ScaleFactor
{
	public: 
	EventScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader)
    : ScaleFactor(name, reader) {}

};

#endif 
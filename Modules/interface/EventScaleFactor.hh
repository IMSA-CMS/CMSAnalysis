#ifndef EVENTSCALEFACTOR_HH
#define EVENTSCALEFACTOR_HH
#include "CMSAnalysis/Modules/interface/MapBasedScaleFactor.hh"

class EventScaleFactor : public MapBasedScaleFactor
{
	public: 
	EventScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader)
    : MapBasedScaleFactor(name, reader) {}

};

#endif 
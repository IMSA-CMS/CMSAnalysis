#include "CMSAnalysis/Plans/interface/CounterPlan.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include <iostream>
#include <memory>
#include "TROOT.h"
#include "TSystem.h"

using std::make_shared;

void CounterPlan::initialize()
{
	auto& modules = getModules();
	//auto eventMod = make_shared<EventModule>();
	//modules.addProductionModule(eventMod);

}
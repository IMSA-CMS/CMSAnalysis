#include "CMSAnalysis/Plans/interface/HiggsSignalPlan.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include <iostream>
#include <memory>
#include "TROOT.h"
#include "TSystem.h"





using std::make_shared;

void HiggsSignalPlan::initialize()
{
    HiggsBackgroundPlan::initialize();
    auto& modules = getModules();
    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    modules.addFilterModule(make_shared<FilterModule>(genSimDecayFilter));
}  
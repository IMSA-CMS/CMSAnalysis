#include "CMSAnalysis/Plans/interface/MuonSignalPlan.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include <memory>

using std::make_shared;

void MuonSignalPlan::initialize()
{
    MuonPlan::initialize();
    auto& modules = getModules();
    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    modules.addFilterModule(make_shared<FilterModule>(genSimDecayFilter));
}  
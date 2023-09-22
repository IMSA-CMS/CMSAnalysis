#include "CMSAnalysis/Modules/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/Plans/interface/HiggsInvariantMassPlan.hh"

void HiggsInvariantMassPlan::initialize()
{
    auto fileMod = std::make_shared<HiggsInvariantMassModule>("invariantMassFile.root");


   	getModules().addAnalysisModule(fileMod);
}
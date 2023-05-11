#include "CMSAnalysis/DataCollection/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsInvariantMassPlan.hh"


void HiggsInvariantMassPlan::initialize()
{
    auto fileMod = std::make_shared<HiggsInvariantMassModule>("invariantMassFile.root");
   	getAnalyzer().addAnalysisModule(fileMod);
}
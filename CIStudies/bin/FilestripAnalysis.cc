#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/DataStripModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/FilestripModule.hh"

using std::make_shared;

Analyzer filestripAnalysis()
{
    Analyzer analyzer;

    //auto filestripMod = make_shared<FilestripModule>();
<<<<<<< HEAD
    auto leptonJetRecoModule = make_shared<LeptonJetReconstructionModule>(0.05);
    auto leptonJetMatchModule = make_shared<LeptonJetMatchingModule>(leptonJetRecoModule, 0.05);
    auto dataStripModule = make_shared<DataStripModule>("LeptonJetData.root", leptonJetRecoModule, leptonJetMatchModule);


    //analyzer.addAnalysisModule(filestripMod);
    analyzer.addProductionModule(leptonJetRecoModule);
    analyzer.addProductionModule(leptonJetMatchModule);
    analyzer.addAnalysisModule(dataStripModule);
=======

    //analyzer.addAnalysisModule(filestripMod);
>>>>>>> 24c4256d1208cc385a5999832387c253248dfd04
    
    return analyzer;
}
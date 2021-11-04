#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/GenSimStudyModule.hh"
#include <memory>

Analyzer genSimStudyAnalysis() {
    Analyzer analyzer;
    auto studyMod = make_shared<GenSimStudyModule>();
    analyzer.addProductionModule(studyMod);
    return analyzer;
}
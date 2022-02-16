#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimStudyModule.hh"
#include <memory>

Analyzer genSimStudyAnalysis() {
    Analyzer analyzer;
    auto studyMod = make_shared<GenSimStudyModule>();
    analyzer.addProductionModule(studyMod);
    return analyzer;
}
#include "CMSAnalysis/DataCollection/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsInvariantMassPlan.hh"
#include "CMSAnalysis/DataCollection/interface/EventModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsCutsSelector.hh"
#include "CMSAnalysis/DataCollection/interface/LocalEventInputModule.hh"


void HiggsInvariantMassPlan::initialize()
{
    // std::cout << "Here 1\n";
    // auto eventMod = std::make_shared<EventModule>();
    // std::cout << "Here 2\n";
    // auto cutSelector = std::make_shared<HiggsCutsSelector>();
    // std::cout << "Here 3\n";
    auto fileMod = std::make_shared<HiggsInvariantMassModule>("invariantMassFile.root");
    // std::cout << "Here 4\n";
    // auto localEventInputMod = std::make_shared<LocalEventInputModule>(&(eventMod->getEvent()));
    // std::cout << "Here 5\n";
    // fileMod->setInput(localEventInputMod.get());
    // std::cout << "Here 6\n";
    // eventMod->addSelector(cutSelector);
    // std::cout << "Here 7\n";
    // getAnalyzer().addAnalysisModule(eventMod);
   	getAnalyzer().addAnalysisModule(fileMod);
    // std::cout << "Here 8\n";
}
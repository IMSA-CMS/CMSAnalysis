#include "CMSAnalysis/Plans/interface/ChargeFlipPlan.hh"
#include <memory>
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

using std::make_shared;


void ChargeFlipPlan::initialize()
{
    auto& modules = getModules();
    auto eventMod = make_shared<EventModule>();
    eventMod->addSelector(make_shared<HiggsSelector>());
    auto matchMod = make_shared<MatchingModule>();
<<<<<<< HEAD


=======
    //matchMod->setInput(eventMod->getEventInput());
>>>>>>> 26e4e24a (Fixed error with electron sign flip efficiency)
    auto signFlip = make_shared<SignFlipModule>(matchMod);
    
    modules.addProductionModule(eventMod);
    modules.addProductionModule(matchMod);
    modules.addAnalysisModule(signFlip);
}


 

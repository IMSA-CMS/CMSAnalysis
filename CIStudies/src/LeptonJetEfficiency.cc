#include "CIAnalysis/CIStudies/interface/LeptonJetEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"

LeptonJetEfficiency::LeptonJetEfficiency(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod):
    lepRecoMod(iLepRecoMod),
    lepMatchMod(iLepMatchMod)

{}

bool LeptonJetEfficiency::process(const edm::EventBase& event)
{
    auto recoLeptonJets = lepRecoMod->getLeptonJets();
    numRecoJets += recoLeptonJets.size();

    auto matchingPairs = lepMatchMod->getMatchingPairs();
    numMatchedJets += matchingPairs.size();

    return true;
}

void LeptonJetEfficiency::finalize()
{
    std::cout << "Lepton Jet Efficiency: " << (double) numMatchedJets / numRecoJets << "\n";
}

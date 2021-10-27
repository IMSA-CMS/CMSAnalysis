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

    for (auto jet : recoLeptonJets) {
        auto particles = jet.getParticles();
        auto zero = [](auto p){return p.hadVetoEt() == 0;};
        bool any = std::any_of(particles.begin(), particles.end(), zero);
        bool all = std::all_of(particles.begin(), particles.end(), zero);
        recoZeroAnyVeto += any;
        recoZeroAllVeto += all;
    }

    return true;
}

void LeptonJetEfficiency::finalize()
{
    std::cout << "Lepton Jet Efficiency: " << (double) numMatchedJets / numRecoJets << "\n";
    std::cout << "Zero Any Lepton Jet Efficiency: " << (double) numMatchedJets / recoZeroAnyVeto << "\n";
    std::cout << "Zero All Lepton Jet Efficiency: " << (double) numMatchedJets / recoZeroAllVeto << "\n";
    std::cout << "total, numMatched, zeroAny, zeroAll\n";
    std::cout << numRecoJets << ", " << numMatchedJets << ", "
        << recoZeroAnyVeto << ", " << recoZeroAllVeto << "\n";
}

#include <cmath>
#include <stdexcept>
#include <fstream>

#include "CMSAnalysis/DataCollection/interface/LeptonJetEfficiency.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"

LeptonJetEfficiency::LeptonJetEfficiency(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod):
    EfficiencyModule(),
    lepRecoMod(iLepRecoMod),
    lepMatchMod(iLepMatchMod)
{}

void LeptonJetEfficiency::doCounters()
{
    auto recoLeptonJets = lepRecoMod->getLeptonJets();
    incrementCounter("Number of Reconstructed Jets", recoLeptonJets.size());
    if (recoLeptonJets.size() > 1) 
    {
        incrementCounter("Multiple Jet Events", 1);
    }
    if (recoLeptonJets.size() == 1)
    {
        incrementCounter("Single Jet Events", 1);
    }
    if (recoLeptonJets.size() == 2)
    {
        incrementCounter("Double Jet Events", 1);
    }
    
    auto matchingPairs = lepMatchMod->getMatchingPairs();
    incrementCounter("Number of matched jets", matchingPairs.size());

}

void LeptonJetEfficiency::finalize()
{
    EfficiencyModule::finalize();

    std::cout << "Lepton Jet Efficiency: " << getCounter("Number of matched jets") / getCounter("Number of Reconstructed Jets") << "\n";
}

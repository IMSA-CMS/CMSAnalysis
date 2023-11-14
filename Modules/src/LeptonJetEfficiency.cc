#include <cmath>
#include <stdexcept>
#include <fstream>

#include "CMSAnalysis/Modules/interface/LeptonJetEfficiency.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"

LeptonJetEfficiency::LeptonJetEfficiency(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod):
    EfficiencyModule(),
    lepRecoMod(iLepRecoMod),
    lepMatchMod(iLepMatchMod)
{
    addRequiredModule(iLepRecoMod);
    addRequiredModule(iLepMatchMod);
}

void LeptonJetEfficiency::doCounters()
{
    auto recoLeptonJets = lepRecoMod->getLeptonJets();
    double muonCount = 0;
    double electronCount = 0;

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
     
    for (auto leptonJet : recoLeptonJets)
    {
        for (auto particle : leptonJet.getParticles())
        {
            if (particle.getType() == ParticleType::electron())
            {
                electronCount++;
            }
            
            if (particle.getType() == ParticleType::muon())
            {
                muonCount++;
            }
            
        }
            //exactly one for both
            //change to if both greater than zero
            //run with diff cone sizes 0.5, 0.3 .1 .01
        if (electronCount > 0 && muonCount > 0)
        {
            for (auto pair : lepMatchMod->getMatchingPairs())
            {
                if (pair.second == leptonJet)
                {
                    incrementCounter("Fake pair match", 1);
                    std::cout << "electon count " << electronCount << "\n";
                    std::cout << "muon count " << muonCount << "\n\n";
                }
            }
            incrementCounter("Number of fake jets", 1);
        }

        

    }

    

    auto matchingPairs = lepMatchMod->getMatchingPairs();
    incrementCounter("Number of matched jets", matchingPairs.size());

}

void LeptonJetEfficiency::finalize()
{
    EfficiencyModule::finalize();

    std::cout << "Lepton Jet Efficiency: " << getCounter("Number of matched jets") / getCounter("Number of Reconstructed Jets") << "\n";
}

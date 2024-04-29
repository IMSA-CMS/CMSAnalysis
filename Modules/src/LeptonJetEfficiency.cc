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
    //do the same thing using mlcalcualtor shared pointeer
{
    addRequiredModule(iLepRecoMod);
    addRequiredModule(iLepMatchMod);
}

void LeptonJetEfficiency::doCounters()
{
    incrementCounter("nEvents", 1);

    //auto recoLeptonJets = lepRecoMod->getLeptonJets();
    auto recoLeptonJets = getInput()->getSpecial("leptonJet");
    auto genSimParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim);
    double muonCount = 0;
    double electronCount = 0;

    incrementCounter("Number of GenSim Jets", 0);
    for (auto particle : genSimParticles)
    {
        GenSimParticle genSimParticle(particle);
        if (particle.getType() == ParticleType::darkPhoton() && genSimParticle == genSimParticle.finalDaughter())
        {
            incrementCounter("Number of GenSim Jets", 1);
        }
    }

    incrementCounter("Number of Reconstructed Jets", recoLeptonJets.size());

    incrementCounter("Multiple Jet Events", 0);
    incrementCounter("Single Jet Events", 0);
    incrementCounter("Jet Events", 0);

    if (recoLeptonJets.size() >= 1) 
    {
        incrementCounter("Jet Events", 1);
    }
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
     
    for (auto leptonJetParticle : recoLeptonJets)
    {
        LeptonJet leptonJet(leptonJetParticle);



        //how to harvest signal and backgroumd

        // how to get value of observation// what do we use for filename liek in mlcalculator 

        //do we want to maxmize (eps)/(sqrt(2.5 + B))

        //do everthing inside here

        //do getoutoput of leptonjet

        //ask cikram foor integral thigny

        for (auto particle : leptonJet.getParticles())
        {
            if (particle.getType() == ParticleType::electron())
            {
                electronCount++;
            }

            //use the mlcalculator output
            //dont loop insane amount of time use finite guesses, one time is one runananalyzer output



            //do here
            //count passed mass > 10
            //change values maybe
            //signal should drop a lot, background should drop little
            
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
                    std::cout << "electron count " << electronCount << "\n";
                    std::cout << "muon count " << muonCount << "\n\n";
                }
            }
            incrementCounter("Number of fake jets", 1);
        }

        // if (leptonJet.getMass() < 10) 
        // {
        //     incrementCounter("Jets Cut", 1);
        // }
        // incrementCounter("Total Jets", 1);
    }

    

    auto matchingPairs = lepMatchMod->getMatchingPairs();
    incrementCounter("Number of matched jets", matchingPairs.size());

}

void LeptonJetEfficiency::finalize()
{
    EfficiencyModule::finalize();

    std::cout << "# Jet Events / Total # Events : " << getCounter("Jet Events") / getCounter("nEvents") << "\n";

    // std::cout << "Matched / Reconstructed Efficiency: " << getCounter("Number of matched jets") / getCounter("Number of Reconstructed Jets") << "\n";
    std::cout << "Reconstructed / # Dark Photons Efficiency: " << getCounter("Number of Reconstructed Jets") / getCounter("Number of GenSim Jets") << "\n";
    // std::cout << "Matched / # Dark Photons Efficiency: " << getCounter("Number of matched jets") / getCounter("Number of GenSim Jets") << "\n";
    // double count = getCounter("Multiple Jet Events") + getCounter("Single Jet Events");
    // std::cout << "Efficiency: " << count / 24400 << "\n";
    // std::cout << "% of Original LJ Efficiency: " << 100 * count / 10879 << "\n";
    // std::cout << "% of Original DY Efficiency: " << 100 * count / 25 << "\n";
}

#include "CMSAnalysis/DataCollection/interface/HPlusPlusGenSimSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void HPlusPlusGenSimSelector::selectParticles(const InputModule *input, Event &event)
{
    auto particles = input->getParticles(InputModule::RecoLevel::GenSim);
    for (const auto &particle : particles)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);
        if ((abs(genSimParticle.pdgId()) == 9900041 || abs(genSimParticle.pdgId()) == 9900042) && genSimParticle == genSimParticle.finalDaughter()) // H++
        {
            if (genSimParticle.numberOfDaughters() == 2 &&
            (abs(genSimParticle.daughter(0).pdgId()) == 13 || abs(genSimParticle.daughter(0).pdgId()) == 11) &&
            (abs(genSimParticle.daughter(1).pdgId()) == 13 || abs(genSimParticle.daughter(1).pdgId()) == 11))
            {
                if (abs(genSimParticle.pdgId()) == 9900041)
                {
                    event.addSpecialObject("LeftHiggs", particle);
                } else {
                    event.addSpecialObject("RightHiggs", particle);
                }
                for (int i = 0; i < 2; i++)
                {
                    if (genSimParticle.daughter(i).getType() == ParticleType::electron())
                    {
                        event.addElectron(genSimParticle.daughter(i));
                    }
                    else if (genSimParticle.daughter(i).getType() == ParticleType::muon())
                    {
                        event.addMuon(genSimParticle.daughter(i));
                    }
                }
            }
        } else if (abs(genSimParticle.pdgId() == 23) && genSimParticle == genSimParticle.finalDaughter()) // Z Boson
        {
            event.addSpecialObject("Zboson", genSimParticle);
        }
    }
}

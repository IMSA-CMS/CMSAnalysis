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
        if ((genSimParticle.getType() == ParticleType::leftDoublyHiggs() || genSimParticle.getType() == ParticleType::rightDoublyHiggs()) && genSimParticle == genSimParticle.finalDaughter()) // H++
        {
            if (genSimParticle.numberOfDaughters() == 2 &&
            (genSimParticle.daughter(0).getType() == ParticleType::electron() || genSimParticle.daughter(0).getType() == ParticleType::muon()) &&
            (genSimParticle.daughter(1).getType() == ParticleType::electron() || genSimParticle.daughter(1).getType() == ParticleType::muon()))
            {
                if (genSimParticle.getType() == ParticleType::leftDoublyHiggs())
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
        } else if (genSimParticle.getType() == ParticleType::z() && genSimParticle == genSimParticle.finalDaughter()) // Z Boson
        {
            event.addSpecialObject("Zboson", genSimParticle);
        }
    }
}

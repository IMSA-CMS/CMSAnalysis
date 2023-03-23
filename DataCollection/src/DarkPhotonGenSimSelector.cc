#include "CMSAnalysis/DataCollection/interface/DarkPhotonGenSimSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void DarkPhotonGenSimSelector::selectParticles(const InputModule* input, Event& event) 
{
    std::vector<Particle> selected;

    auto particles = input->getParticles(InputModule::RecoLevel::GenSim);

    for (const auto& particle : particles)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);
        if (genSimParticle.getType() == ParticleType::darkPhoton() && genSimParticle == genSimParticle.finalDaughter()) //Dark Photon
        {
            if (genSimParticle.numberOfDaughters() == 2 && 
            (genSimParticle.daughter(0).getType() == ParticleType::electron() || genSimParticle.daughter(0).getType() == ParticleType::muon()) && 
            (genSimParticle.daughter(1).getType() == ParticleType::electron() || genSimParticle.daughter(1).getType() == ParticleType::muon()))
            {
                event.addSpecialObject("DarkPhoton",genSimParticle);
                for (int i = 0; i < 2; i++)
                {
                    if (genSimParticle.daughter(i).getType() == ParticleType::electron())
                    {
                        event.addElectron(Electron(genSimParticle.daughter(i).finalDaughter()));
                    } else if (genSimParticle.daughter(i).getType() == ParticleType::muon())
                    {
                        event.addMuon(Muon(genSimParticle.daughter(i).finalDaughter()));
                    }
                }
            }
        }
        if (genSimParticle.getType() == ParticleType::neutralino() && genSimParticle == genSimParticle.finalDaughter()) //neutralino
        {
            event.addSpecialObject("Neutralino",genSimParticle);
        }
        if (genSimParticle.getType() == ParticleType::z() && genSimParticle == genSimParticle.finalDaughter()) //Z Boson
        {
            event.addSpecialObject("Zboson",genSimParticle);
        }
    }
}

std::vector<Particle> DarkPhotonGenSimSelector::checkJet(GenSimParticle part) const //recursively cycles through jets to find leptons
{
    std::vector<Particle> selected;
    if (part.isFinalState())
    {
        if (part.getType() == ParticleType::electron() || part.getType() == ParticleType::muon())
        {
            selected.push_back(part);
        }
    } 
    else 
    {
        for (int i = 0; i < part.numberOfDaughters(); i++) 
        {
            auto daughters = checkJet(part.daughter(i));
            selected.insert(selected.end(),daughters.begin(),daughters.end());
        }
    }
    return selected;
}
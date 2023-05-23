#include "CMSAnalysis/Filters/interface/DarkPhotonGenSimSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

void DarkPhotonGenSimSelector::selectParticles(const InputModule* input, Event& event) 
{
    std::vector<Particle> selected;

    auto particles = input->getParticles(InputModule::RecoLevel::GenSim);

    for (const auto& particle : particles)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);
        if (genSimParticle.pdgId() == 4900022 && genSimParticle == genSimParticle.finalDaughter()) //Dark Photon
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
        if (abs(genSimParticle.pdgId() == 1000022) && genSimParticle == genSimParticle.finalDaughter()) //neutralino
        {
            if (genSimParticle.numberOfDaughters() == 2 && genSimParticle.daughter(0).pdgId() != 4900002 && genSimParticle.daughter(1).pdgId() != 4900002)
            { //4900002 means no jet
                event.addSpecialObject("Neutralino",genSimParticle);
                // for (const auto& part : checkJet(genSimParticle))
                // {
                //     if (part.getType() == ParticleType::electron())
                //     {
                //         event.addElectron(Electron(GenSimParticle(part).finalDaughter()));
                //     } 
                //     else if (part.getType() == ParticleType::muon())
                //     {
                //         event.addMuon(Muon(GenSimParticle(part).finalDaughter()));
                //     }
                // }
            }
        }
        if (abs(genSimParticle.pdgId() == 23) && genSimParticle == genSimParticle.finalDaughter()) //Z Boson
        {
            event.addSpecialObject("Zboson",genSimParticle);
        }
    }
}

std::vector<Particle> DarkPhotonGenSimSelector::checkJet(GenSimParticle part) const //cycles through jets to find leptons
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
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
                        event.addElectron(Electron(genSimParticle.daughter(i)));
                    } else if (genSimParticle.daughter(i).getType() == ParticleType::muon())
                    {
                        event.addMuon(Muon(genSimParticle.daughter(i)));
                    }
                }
            }
        }
        if (abs(genSimParticle.pdgId() == 1000022) && genSimParticle == genSimParticle.finalDaughter()) //neutralino
        {
            if (genSimParticle.numberOfDaughters() == 2 && genSimParticle.daughter(0).pdgId() != 4900002 && genSimParticle.daughter(1).pdgId() != 4900002)
            { //4900002 means no jet
                event.addSpecialObject("Neutralino",genSimParticle);
                for (const auto& part : checkJet(genSimParticle))
                {
                    if (part.getType() == ParticleType::electron())
                    {
                        event.addElectron(Electron(part));
                    } 
                    else if (part.getType() == ParticleType::muon())
                    {
                        event.addMuon(Muon(part));
                    }
                }
            }
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
            selected.insert(selected.end(),checkJet(part.daughter(i)).begin(),checkJet(part.daughter(i)).end());
        }
    }
    return selected;
}
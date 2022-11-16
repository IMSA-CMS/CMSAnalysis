#include "CMSAnalysis/DataCollection/interface/PASSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void PASSelector::selectParticles(const InputModule* input, Event& event)
{
    std::vector<Particle> selected;
    // std::vector<Particle> posElecs(0);
    // std::vector<Particle> negElecs(0);
    // std::vector<Particle> posMuons(0);
    // std::vector<Particle> negMuons(0);
    auto particles = input->getLeptons(InputModule::RecoLevel::Reco).getParticles();

    
    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron() && particle.getPt() > 40 && std::abs(particle.getEta()) < 3) {

            if (Lepton(particle).isLoose())
            {
                event.addElectron(particle);
            }
        }
        if (particle.getType() == ParticleType::muon() && particle.getPt() > 40 && std::abs(particle.getEta()) < 2.8) 
        {
            if(Lepton(particle).isLoose())
            {
                event.addMuon(particle);
            }
        }
    }
}
#include "CMSAnalysis/DataCollection/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

void LeptonJetSelector::selectParticles(const InputModule* input, Event& event)
{
    std::vector<Particle> selected;
    auto particles = input->getLeptons(InputModule::RecoLevel::Reco).getParticles();
    
    
    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::muon() && particle.getPt() > 5) 
        {
            if(Lepton(particle).isLoose())
            {
                event.addMuon(particle);
            }
        }
    }
}


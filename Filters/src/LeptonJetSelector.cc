#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

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


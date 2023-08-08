#include "CMSAnalysis/Filters/interface/PASSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

void PASSelector::selectParticles(const EventInput* input, Event& event) const
{
    std::vector<Particle> selected;
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();

    
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

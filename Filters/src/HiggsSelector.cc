#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

void HiggsSelector::selectParticles(const EventInput* input, Event& event) const
{
    std::cout << "Getting leptons\n";
    std::cout << "Print electron id" << ParticleType::electron().getpdgId() << '\n';
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();

    for (const auto& particle : particles)
    {
        std::cout << particle;
    }

    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron()) 
		{
            auto lepton = Lepton(particle);
            if(lepton.isLoose()
                && particle.getPt() > 5
                && particle.getInfo("CutBasedHEEP")
            )
            {
                event.addElectron(particle);
            }
        }
        else if (particle.getType() == ParticleType::muon())
        {
            auto lepton = Lepton(particle);
            if (lepton.isLoose()  
                && particle.getPt() > 5
                && particle.getInfo("Isolation") < 0.05
                && lepton.getDXY() < 0.025
                && lepton.getDZ() < 0.05
            )
            {
                event.addMuon(particle);
            }            
        }
    }
}
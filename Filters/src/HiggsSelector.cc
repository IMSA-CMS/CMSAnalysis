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
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();

    std::vector<Particle> leptons;

    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron()) 
		{
            auto lepton = Lepton(particle);
            if(lepton.isLoose()
            && particle.getPt() > 10
                
            )
            {
                //std::cout << "PT: " << std::to_string(particle.getPt()) << std::endl;
                leptons.push_back(particle);
            }
        }
        else if (particle.getType() == ParticleType::muon())
        {
            //std::cout << "In Muon Selection" << std::endl;
            auto lepton = Lepton(particle);
            if (lepton.isTight()  
                && particle.getPt() > 5
                // && particle.getInfo("Isolation") < 0.05
                // && lepton.getDXY() < 0.025
                // && lepton.getDZ() < 0.05
            )
            {
                leptons.push_back(particle);
            }
        }
    }

    while (leptons.size() > 4)
    {
        std::vector<double> pTs;
        for (auto particle : leptons)
        {
            pTs.push_back(particle.getPt());
        }
        double minPt = *min_element(pTs.begin(), pTs.end());
        int index = find(pTs.begin(), pTs.end(), minPt) - pTs.begin();
        leptons.erase(leptons.begin() + index);
    }

    for (auto particle : leptons)
    {
        if (particle.getType() == ParticleType::electron())
        {
            event.addElectron(particle);
        }
        else if (particle.getType() == ParticleType::muon())
        {
            //std::cout << particle.getInfo("Isolation") << std::endl;
            event.addMuon(particle);
        }
    }
}
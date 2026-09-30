#include "CMSAnalysis/Filters/interface/KansasCuts.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
//nanoaod stuff --> changed the cuts for electrons and muons specifically


 void KansasCuts::selectParticles(const EventInput* input, Event& event) const
{
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();


    std::vector<Particle> leptons;
    std::vector<Particle> positiveLeptons;
    std::vector<Particle> negativeLeptons;

    for (const auto& particle : particles)
    {
		double deltaR = std::numeric_limits<double>::max();
		for (const auto& particle2 : particles)
    	{	
			if (particle == particle2 || particle.getType() == ParticleType::tau())
			{
				continue;
			}
			double deltaR_calc = particle.getDeltaR(particle2);
			if (deltaR_calc < deltaR)
			{
				deltaR = deltaR_calc;
			}
		}
        if (particle.getType() == ParticleType::electron())
     {
            auto lepton = Lepton(particle);
            if(lepton.isLoose()
            /* && particle.getPt() > 38 //do something similar for the other variables in kansas file 
            && particle.getInfo("Isolation") < 0.1
			&& std::abs(particle.getEta()) < 2.5
			&& particle.getInfo("dxy") < 0.045
			&& particle.getInfo("dz") < 0.2
			&& particle.getInfo("mva90") 
			&& deltaR > 0.3 */
            ) 
            { 
                //std::cout << "PT: " << std::to_string(particle.getPt()) << std::endl;
                leptons.push_back(particle);
                if (lepton.getCharge() > 0)
                {
                    positiveLeptons.push_back(particle);
                }
                else
                {
                    negativeLeptons.push_back(particle);
                }
            }
        }
        else if (particle.getType() == ParticleType::muon())
        {
            //std::cout << "In Muon Selection" << std::endl;
            auto lepton = Lepton(particle);
            if (lepton.isTight()  
                && particle.getPt() > 30 //need to know if this is correct
                && particle.getInfo("Isolation") < 0.15
				&& std::abs(particle.getEta()) < 2.4
                && particle.getInfo("dxy") < 0.2
				&& particle.getInfo("dz") < 0.5
				&& particle.getInfo("mtight") 
				&& deltaR > 0.3

            )
            {
                leptons.push_back(particle);
            }
        }
        
        else if (particle.getType() == ParticleType::tau()) //ask what to do about the tight, medium, loose
        {
        double deltaR_tt = std::numeric_limits<double>::max(); // tau–tau
        double deltaR_lt = std::numeric_limits<double>::max(); // lepton–tau

        for (const auto& other : particles)
        {
            if (other == particle) continue;

            double dR = particle.getDeltaR(other);

            if (other.getType() == ParticleType::tau())
            {
                deltaR_tt = std::min(deltaR_tt, dR);
            }
            else if (other.getType() == ParticleType::electron() ||
                    other.getType() == ParticleType::muon())
            {
                deltaR_lt = std::min(deltaR_lt, dR);
            }
        }
            auto lepton = Lepton(particle);
            //need another loop over particle. if its a tau loop over all particles again, if one of the particles is an election or muon, then you want to calculate delta R for each particle. There's already a function for it (Particle.hh). If the delta r is less than some value we set, ten we just say that they're the same particle. If number is too small just continue. 
            if (lepton.isTight() 
            //&& particle.getPt() > 20 
            //&& std::abs(particle.getEta()) < 2.3 
             //&& particle.getInfo("dz") < 0.5 
            //&& static_cast<int>(particle.getInfo("tau_idVSmu")) & 4
            //&& static_cast<int>(particle.getInfo("tau_idVSele")) & 8
            && deltaR_tt > 0.5
            // && deltaR_lt > 0.5 
            )
            {
                //bool overlap = false;

                // loop over all other particles (from input, not just taus)
                // for (const auto& other : particles)
                // {
                //     if (other == particle) continue; // skip self

                //     // only check against electrons and muons
                //     if (other.getType() == ParticleType::electron() ||
                //         other.getType() == ParticleType::muon())
                //     {
                //         double dR = particle.getDeltaR(other); // Particle.hh already has this
                //         if (dR < 0.3) // typical cone cut, can tune to 0.2–0.4
                //         {
                //             overlap = true;
                //             break;
                //         }
                //     }
                // }

                //if (!overlap)
                {
                    leptons.push_back(particle); // only keep tau if no e/μ is too close
                }
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

    //auto correctedLeptons = adjustForNeutrinos(positiveLeptons, negativeLeptons, input);
    auto correctedLeptons = leptons; // Temporarily disable neutrino adjustment
    for (auto particle : correctedLeptons)
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
        else if (particle.getType() == ParticleType::tau())
        {
            //std::cout << particle.getInfo("Isolation") << std::endl;
            event.addTau(particle);
        }
    }
}


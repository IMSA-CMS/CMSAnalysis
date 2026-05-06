#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

void HPlusPlusGenSimSelector::selectParticles(const EventInput *input, Event &event) const
{
    auto particles = input->getParticles(EventInput::RecoLevel::GenSim);
    bool plus = false;
    bool minus = false;

    // std::cout << "-" << std::endl;

    for (const auto &particle : particles)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);

        // std::cout << particle.getName() << " (" << genSimParticle.pdgId() << "): " << std::to_string(particle.getCharge()) << std::endl;

        // if(particle.getType() == ParticleType::electron() || particle.getType() == ParticleType::muon())
        // {
        //     std::cout << particle.getName() << " (" << genSimParticle.pdgId() << "): " << std::to_string(particle.getCharge()) << std::endl;
        //     std::cout << "Eta: " << genSimParticle.getEta() << std::endl;
        //     std::cout << "pT: " << genSimParticle.getPt() << std::endl;
        // }

        if ((genSimParticle.getType() == ParticleType::leftDoublyHiggs() || genSimParticle.getType() == ParticleType::rightDoublyHiggs()) && genSimParticle == genSimParticle.finalDaughter()) // H++
        {
            if (genSimParticle.numberOfDaughters() == 2 && (genSimParticle.daughter(0).getType() == ParticleType::electron() || genSimParticle.daughter(0).getType() == ParticleType::muon() || genSimParticle.daughter(0).getType() == ParticleType::tau()) && (genSimParticle.daughter(1).getType() == ParticleType::electron() || genSimParticle.daughter(1).getType() == ParticleType::muon() || genSimParticle.daughter(1).getType() == ParticleType::tau()))
            {
                if (genSimParticle.getType() == ParticleType::leftDoublyHiggs())
                {
                    // event.addSpecialObject("LeftHiggs", particle);
                    // event.addGenSimParticle(genSimParticle);
                    if (genSimParticle.pdgId() > 0 && !plus)
                    {
                        plus = true;
                        // event.addSpecialObject("LeftHiggs", particle);
                        event.addGenSimParticle(genSimParticle);
                    }
                    else if (genSimParticle.pdgId() < 0 && !minus)
                    {
                        minus = true;
                        // event.addSpecialObject("LeftHiggs", particle);
                        event.addGenSimParticle(genSimParticle);
                    }
                } else {
                    // event.addSpecialObject("RightHiggs", particle);
                    // event.addGenSimParticle(genSimParticle);
                    if (genSimParticle.pdgId() > 0 && !plus)
                    {
                        plus = true;
                        // event.addSpecialObject("RightHiggs", particle);
                        event.addGenSimParticle(genSimParticle);
                    }
                    else if (genSimParticle.pdgId() < 0 && !minus)
                    {
                        minus = true;
                        // event.addSpecialObject("RightHiggs", particle);
                        event.addGenSimParticle(genSimParticle);
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    event.addGenSimParticle(genSimParticle.daughter(i));
                }
            }
        } else if (genSimParticle.getType() == ParticleType::z() && genSimParticle == genSimParticle.finalDaughter()) // Z Boson
        {
            event.addSpecialObject("Zboson", genSimParticle);
            event.addGenSimParticle(genSimParticle);
        }
        // Drell-Yan
        // else if (genSimParticle.getType() == ParticleType::photon())
        // {
        //     if (genSimParticle.numberOfDaughters() == 2 && (genSimParticle.daughter(0).getType() == ParticleType::electron() || genSimParticle.daughter(0).getType() == ParticleType::muon()) && (genSimParticle.daughter(1).getType() == ParticleType::electron() || genSimParticle.daughter(1).getType() == ParticleType::muon()))
        //     {
        //         for (int i = 0; i < 2; i++)
        //         {
        //             event.addGenSimParticle(genSimParticle.daughter(i));
        //         }
        //     }
        // }
        else if (genSimParticle.getType() == ParticleType::electron() && genSimParticle.getPt() > 5)
        {
            event.addGenSimParticle(genSimParticle);
        }
        else if (genSimParticle.getType() == ParticleType::muon() && genSimParticle.getPt() > 5)
        {
            // genSimParticle.addInfo("Isolation", 0); //Dummy value for isolation for errors
            event.addGenSimParticle(genSimParticle);
        }
    }
}

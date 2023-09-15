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
    std::vector<Particle> selected;
    std::vector<Particle> posElecs;
    std::vector<Particle> negElecs;
    std::vector<Particle> posMuons;
    std::vector<Particle> negMuons;
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();

    std::vector<Particle> leptons;

    // int posLeptonCount = 0;
    // int negLeptonCount = 0;

    int electronCount = 0;
    int posMuonCount = 0;
    int negMuonCount = 0;
    
    // std::cout << "-----" << std::endl;
    
    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron()) 
		{
            auto lepton = Lepton(particle);
            if(lepton.isLoose()
                && particle.getPt() > 5 
                // && std::abs(particle.getDXY()) < 0.01
                // && std::abs(particle.getDZ()) < 0.025
            )
            {
                event.addElectron(particle);
                // leptons.push_back(particle);
                // if (particle.getCharge() > 0)
                // {
                //     posLeptonCount++;
                // }
                // else
                // {
                //     negLeptonCount++;
                // }

                // std::cout << particle.getName() << ": " << std::to_string(particle.getCharge()) << std::endl;

                electronCount++;
            
                if (particle.getCharge() > 0) {
                    posElecs.push_back(particle);
                } 
                else {
                    negElecs.push_back(particle);
                }
            }
        }
        else if (particle.getType() == ParticleType::muon()) // if (particle.getType() == Particle::Type::Muon && particle.getPt() > 40 && std::abs(particle.getEta()) < 2.8) 
        {
            auto lepton = Lepton(particle);
            if(lepton.isLoose()  
                && particle.getPt() > 5 
                // && std::abs(particle.getDXY()) < 0.003125
                // && std::abs(particle.getDZ()) < 0.007183
            )
            {
                event.addMuon(particle);
                // leptons.push_back(particle);
                // if (particle.getCharge() > 0)
                // {
                //     posLeptonCount++;
                // }
                // else
                // {
                //     negLeptonCount++;
                // }

                // std::cout << particle.getName() << ": " << std::to_string(particle.getCharge()) << std::endl;
                if (particle.getCharge() > 0) {
                    posMuonCount++;
                    posMuons.push_back(particle);
                } 
                else {
                    negMuonCount++;
                    negMuons.push_back(particle);
                }
            }
        }
    }

    // std::cout << "-" << std::endl;
    
    // int index1 = -1;
    // int index2 = -1;
    // double largestMass = 0.0f;
    // if (posElecs.size() > 0) 
    // {
    //     for (int i = 0; i < posElecs.size() - 1; i++) 
    //     {
    //         for (int j = i+1; j < int(posElecs.size()); j++)
    //         {
    //             if (posElecs[i].getType() == posElecs[j].getType() && (posElecs[i].getFourVector() + posElecs[j].getFourVector()).M() > largestMass)
    //             {
    //                 index1 = i;
    //                 index2 = j;
    //                 largestMass = (posElecs[i].getFourVector() + posElecs[j].getFourVector()).M();
                    
    //             }
    //         }
    //     }
    //     selected.push_back(posElecs[index1]);
    //     selected.push_back(posElecs[index2]);
    // }
    // index1 = -1;
    // index2 = -1;
    // largestMass = 0.0f;
    // if (negElecs.size() > 0)
    // {
    //     for (int i = 0; i < negElecs.size() - 1; i++) 
    //     {
    //         for (int j = i+1; j < negElecs.size(); j++)
    //         {
    //             if (negElecs[i].getType() == negElecs[j].getType() && (negElecs[i].getFourVector() + negElecs[j].getFourVector()).M() > largestMass)
    //             {
    //                 index1 = i;
    //                 index2 = j;
    //                 largestMass = (negElecs[i].getFourVector() + negElecs[j].getFourVector()).M();
    //             }
    //         }
    //     }
    //     selected.push_back(negElecs[index1]);
    //     selected.push_back(negElecs[index2]);
    // }

    // index1 = -1;
    // index2 = -1;

    // if (posMuonCount > 0)
    // {
    //     for (int i = 0; i < posMuonCount - 1; i++) 
    //     {
    //         for (int j = i + 1; j < posMuonCount; j++)
    //         {
    //             if (posMuons[i].getType() == posMuons[j].getType() && (posMuons[i].getFourVector() + posMuons[j].getFourVector()).M() > largestMass)
    //             {
    //                 index1 = i;
    //                 index2 = j;
    //                 largestMass = (posMuons[i].getFourVector() + posMuons[j].getFourVector()).M();
                    
    //             }
    //         }
    //     }
    //     if (index1 != -1 && index2 != -1)
    //     {
    //         selected.push_back(posMuons[index1]);
    //         selected.push_back(posMuons[index2]);
    //     }
    // }
    // index1 = -1;
    // index2 = -1;
    // largestMass = 0.0f;
    // if (negMuonCount > 0)
    // {
    //     for (int i = 0; i < negMuonCount - 1; i++) 
    //     {
    //         for (int j = i + 1; j < negMuonCount; j++)
    //         {
    //             if (negMuons[i].getType() == negMuons[j].getType() && (negMuons[i].getFourVector() + negMuons[j].getFourVector()).M() > largestMass)
    //             {
    //                 index1 = i;
    //                 index2 = j;
    //                 largestMass = (negMuons[i].getFourVector() + negMuons[j].getFourVector()).M();
    //             }
    //         }
    //     }
    //     if (index1 != -1 && index2 != -1)
    //     {
    //         selected.push_back(negMuons[index1]);
    //         selected.push_back(negMuons[index2]);
    //     }
    // }

    // if (posMuonCount + negMuonCount < 4 && electronCount < 4)
    // {
    //     return std::vector<Particle>(0);
    // }

    // for (auto &part : posElecs)
    // {
    //     selected.push_back(part);
    // }
    // for (auto &part : negElecs)
    // {
    //     selected.push_back(part);
    // }
    // for (auto &part : posMuons)
    // {
    //     selected.push_back(part);
    // }
    // for (auto &part : negMuons)
    // {
    //     selected.push_back(part);
    // }
}
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

    // int electronCount = 0;
    // int posMuonCount = 0;
    // int negMuonCount = 0;
    
    
    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron() && particle.getPt() > 40 && std::abs(particle.getEta()) < 3) {
            if (Lepton(particle).isLoose())
            {
                selected.push_back(particle);
            }
            // electronCount++;

            // if (particle.getCharge() > 0) {
            //     posElecs.push_back(Particle(particle));
            // } 
            // else {
            //     negElecs.push_back(Particle(particle));
            // }
        }
        if (particle.getType() == ParticleType::muon() && particle.getPt() > 40 && std::abs(particle.getEta()) < 2.8) 
        {
            if(Lepton(particle).isLoose())
            {
                selected.push_back(particle);
            }
            // if (particle.getCharge() > 0) {
            //     posMuonCount++;
            //     posMuons.push_back(Particle(particle));
            // } 
            // else {
            //     negMuonCount++;
            //     negMuons.push_back(Particle(particle));
            // }
        }
    }
    
    // int index1 = 0;
    // int index2 = 0;
    // double largestMass = 0.0f;
    // if (posElecs.size() > 0) 
    // {
    //     for (int i = 0; i < int(posElecs.size()) - 1; i++) 
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
    // index1 = 0;
    // index2 = 0;
    // largestMass = 0.0f;
    // if (negElecs.size() > 0)
    // {
    //     for (int i = 0; i < int(negElecs.size()) - 1; i++) 
    //     {
    //         for (int j = i+1; j < int(negElecs.size()); j++)
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

    // index1 = 0;
    // index2 = 0;

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
    //     selected.push_back(posMuons[index1]);
    //     selected.push_back(posMuons[index2]);
    // }
    // index1 = 0;
    // index2 = 0;
    // largestMass = 0.0f;
    // if (negMuonCount > 0)
    // {
    //     for (int i = 0; i < negMuonCount-1; i++) 
    //     {
    //         for (int j = i+1; j < negMuonCount; j++)
    //         {
    //             if (negMuons[i].getType() == negMuons[j].getType() && (negMuons[i].getFourVector() + negMuons[j].getFourVector()).M() > largestMass)
    //             {
    //                 index1 = i;
    //                 index2 = j;
    //                 largestMass = (negMuons[i].getFourVector() + negMuons[j].getFourVector()).M();
    //             }
    //         }
    //     }
    //     selected.push_back(negMuons[index1]);
    //     selected.push_back(negMuons[index2]);
    // }
    // if (posMuonCount + negMuonCount < 4 && electronCount < 4)
    // {
    //     return std::vector<Particle>(0);
    // }

    //return selected;
}
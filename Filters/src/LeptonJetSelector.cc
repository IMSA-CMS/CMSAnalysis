#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"

void LeptonJetSelector::selectParticles(const InputModule* input, Event& event) const
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

    // add jet loop here
    auto recoLeptons = event.getMuons();

// THIS NEEDS TO BE FIXED

    // auto leptonJets = recoMod->findLeptonJets(recoLeptons);


    // // add leptons to the event addSpecialObject
    // for (const auto& jet : leptonJets)
    // {
    //     event.addSpecialObject("leptonJet", jet, InputModule::RecoLevel::Reco);
    // }

    // then cut is easier (add directional cut) *elsewhere

}


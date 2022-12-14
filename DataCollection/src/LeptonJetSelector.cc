#include "CMSAnalysis/DataCollection/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
// #include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

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

    // add jet loop here
    auto recoLeptons = event.getMuons();
    auto leptonJets = recoMod->findLeptonJets(recoLeptons);


    // add leptons to the event addSpecialObject
    for (const auto& jet : leptonJets)
    {
        event.addSpecialObject("leptonJet", jet);
    }

    // then cut is easier (add directional cut) *elsewhere

}


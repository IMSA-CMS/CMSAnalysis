#include "CMSAnalysis/Modules/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "TFile.h"

HiggsInvariantMassModule::HiggsInvariantMassModule(std::string iname):
    name(iname)
{
}

void HiggsInvariantMassModule::initialize()
{
    file = new TFile(name.c_str(), "RECREATE");
    tree = new TTree("invariantMass", "Higgs Invariant Mass");
    //tree->SetAutoSave(0);

    tree->Branch("invariantMass", &muonInvariantMass);
    tree->Branch("muonSize", &muonSize);
}

bool HiggsInvariantMassModule::process()
{
// loop through, make a collection that has pt greater than 5
    auto muons = getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());
    ParticleCollection highPtMuons;

    for (const auto& _muon : muons)
    {
        if (_muon.getPt() > 5)
        {
            highPtMuons.addParticle(_muon);
        }
    }

    muonSize = highPtMuons.getNumParticles();

    if (muonSize < 4)
    {
        return true;
    }

    muonInvariantMass = highPtMuons.calculateSameSignInvariantMasses();

    
    file->cd();
    tree->Fill();
    muonInvariantMass.clear();

    return true;
}

void HiggsInvariantMassModule::finalize()
{
	file->Write();
	file->Close();
}
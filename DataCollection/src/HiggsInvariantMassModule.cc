#include "CMSAnalysis/DataCollection/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"
#include "TFile.h"

HiggsInvariantMassModule::HiggsInvariantMassModule(std::string iname):
    name(iname)
{
}

void HiggsInvariantMassModule::initialize()
{
    file = new TFile(name.c_str(), "RECREATE");
    tree = new TTree("invariantMass", "Higgs Invariant Mass");
    tree->SetAutoSave(0);

    tree->Branch("invariantMass", &muonInvariantMass);
}

bool HiggsInvariantMassModule::process()
{
    auto muons = getInput()->getParticles(InputModule::RecoLevel::Reco, ParticleType::muon());
    muonSize = muons.getNumParticles();
    muonInvariantMass = muons.calculateSameSignInvariantMasses();
    
    file->cd();
    tree->Fill();
    muonInvariantMass.clear();

    return true;
}

void HiggsInvariantMassModule::writeAll()
{
	file->Write();
	file->Close();
}
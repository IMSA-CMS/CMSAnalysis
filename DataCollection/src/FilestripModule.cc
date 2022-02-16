/*
#include "CMSAnalysis/DataCollection/interface/FilestripModule.hh"

FilestripModule::FilestripModule()
{
     file = new TFile("test.root", "recreate");
    tree = new TTree("StrippedFile", "a stripped-down version of MiniAOD");
    // tree->AddAllocationCount(UInt_t(100000));
    tree->Branch("Muons", &muons);
    // tree->Branch("Electron", &electrons);
     tree->Branch("METs", &mets);
     tree->Branch("GenParticles", &genParticles);
}

bool FilestripModule::process()
{
    //edm::Handle<std::vector<pat::Muon>> muonList;
    //event.getByLabel(edm::InputTag("slimmedMuons"), muonList);
    auto muonList = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon).getParticles();
    muons.clear();
    for (const auto& p : muonList)
	{  
        if (p.pt() > 5) // && std::abs(p.eta()) < 2)  
        {    
            muons.push_back(p);
        }
    }
    
     
    
    / *
    edm::Handle<std::vector<pat::Electron>> electronList;
    event.getByLabel(edm::InputTag("slimmedElectrons"), electronList);
    electrons.clear();
    for (const auto& p : *electronList)
	{       
        electrons.push_back(p);
    } 
    * /

    
    //edm::Handle<std::vector<pat::MET>> metList;
    //event.getByLabel(edm::InputTag("slimmedMETs"), metList);
    auto metList = getInput()->getMET();
    mets.clear();
    for (const auto& p : metList)
	{       
        mets.push_back(p);
    } 
    

    
    //edm::Handle<std::vector<reco::GenParticle>> genParticleList;
    //event.getByLabel(edm::InputTag("prunedGenParticles"), genParticleList);
    auto genParticleList = getInput()->getParticles(RecoLevel::GenSim)
    genParticles.clear();
    for (const auto& p : genParticleList)
	{       
        genParticles.push_back(p);
    }
    

    tree->Fill(); 
    return true;
}

void FilestripModule::writeAll()
{
    // tree->Write();
     file->Write();
}
*/

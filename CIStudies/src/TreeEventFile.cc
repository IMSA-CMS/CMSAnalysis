// MORE CHANGES NEEDED FOR IT TO WORK
// SO FAR I JUST COPIED FROM MINIAODEVENTLOADER
// Andy, [10/06/21]

#include "TTree.h"
#include "CIAnalysis/CIStudies/interface/TreeEventFile.hh"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"

TreeEventFile::TreeEventFile(TFile* ifile) : 
EventFile(ifile) {}

void TreeEventFile::initialize()
{
    tree = getFile()->Get<TTree>(getTreeName().c_str());
    // Set branch addresses immediately
    tree->SetBranchAddress(getTreeBranches().elecSize.c_str(), &elec_size);
    tree->SetBranchAddress(getTreeBranches().elecEta.c_str(), &elec_eta);
    tree->SetBranchAddress(getTreeBranches().elecPhi.c_str(), &elec_phi);
    tree->SetBranchAddress(getTreeBranches().elecMass.c_str(), &elec_mass);
    tree->SetBranchAddress(getTreeBranches().elecCharge.c_str(), &elec_charge);
    tree->SetBranchAddress(getTreeBranches().elecPT.c_str(), &elec_pt);

    tree->SetBranchAddress(getTreeBranches().muonSize.c_str(), &muon_size);
    tree->SetBranchAddress(getTreeBranches().muonEta.c_str(), &muon_eta);
    tree->SetBranchAddress(getTreeBranches().muonPhi.c_str(), &muon_phi);
    tree->SetBranchAddress(getTreeBranches().muonMass.c_str(), &muon_mass);
    tree->SetBranchAddress(getTreeBranches().muonCharge.c_str(), &muon_charge);
    tree->SetBranchAddress(getTreeBranches().muonPT.c_str(), &muon_pt);

    
    tree->SetBranchAddress(getTreeBranches().metSize.c_str(), &met_size);
    tree->SetBranchAddress(getTreeBranches().metPhi.c_str(), &met_phi);
    tree->SetBranchAddress(getTreeBranches().metPT.c_str(), &met_pt);

    counter = 0;
    tree->GetEntry(counter);
    ++(counter);

    /* // Now loop through by index to collect everything??
    Int_t nentries = (Int_t)t1->GetEntries();
    for(int i = 0; i < nentries; i++) {
        // Loop through using getEntry
    } */
}

void TreeEventFile::nextEvent()
{
    // Need to call tree->GetEntry(new #)
    // std::cout << counter << std::endl;
    tree->GetEntry(counter);
    // std::cout << "got entry" << std::endl;
    ++(counter);
    setEventCount(getEventCount() + 1);
}

GenEventInfoProduct TreeEventFile::getGenInfo() const
{
    throw std::runtime_error("Not yet implemented: getGenInfo()");

    /* 
    edm::Handle<GenEventInfoProduct> genInfo;
    event->getByLabel(std::string("generator"), genInfo);
    return *genInfo;
    */
}

ParticleCollection TreeEventFile::getGenSimParticles() const
{
    throw std::runtime_error("Not yet implemented: getGenSimParticles()");

    /*
    ParticleCollection genParticles;
    edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
    event->getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
    for (const auto &p : *genParticlesHandle) 
    {
        genParticles.addParticle(Particle(&p));
    }
    return genParticles;
    */
}

ParticleCollection TreeEventFile::getRecoParticles() const
{
    ParticleCollection recoParticles;
    //This seems problematic
    // std::cout << elec_size << std::endl; 
    // std::cout << muon_size << std::endl; 
        
    for(int i = 0; i < elec_size; i++) {
        // Lorentz four-vector
        /* if(i == 0) {
            std::cout << elec_pt[i] << " " << elec_eta[i] << " " <<
             elec_phi[i] << " " << elec_mass[i] << std::endl;
        } */
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elec_pt[i], 
            elec_eta[i], elec_phi[i], elec_mass[i])), elec_charge[i], Particle::Type::Electron));        
    }


    for(int i = 0; i < muon_size; i++)
    {       
        // Lorentz four-vector
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muon_pt[i], 
            muon_eta[i], muon_phi[i], muon_mass[i])), muon_charge[i], Particle::Type::Muon));        
    }

    return recoParticles;
}

double TreeEventFile::getMET() const
{
    return static_cast<double>(met_pt[0]);
}

bool TreeEventFile::isDone() const
{
    return counter >= tree->GetEntries();
}
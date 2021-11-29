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

    tree->SetBranchAddress(getTreeBranches().jetSize.c_str(), &jet_size);
    tree->SetBranchAddress(getTreeBranches().jetEta.c_str(), &jet_eta);
    tree->SetBranchAddress(getTreeBranches().jetPhi.c_str(), &jet_phi);
    tree->SetBranchAddress(getTreeBranches().jetMass.c_str(), &jet_mass);
    tree->SetBranchAddress(getTreeBranches().jetPT.c_str(), &jet_pt);

    tree->SetBranchAddress(getTreeBranches().genSize.c_str(), &gen_size);
    tree->SetBranchAddress(getTreeBranches().genPid.c_str(), &gen_pid);
    tree->SetBranchAddress(getTreeBranches().genStatus.c_str(), &gen_status);
    tree->SetBranchAddress(getTreeBranches().genEta.c_str(), &gen_eta);
    tree->SetBranchAddress(getTreeBranches().genPhi.c_str(), &gen_phi);
    tree->SetBranchAddress(getTreeBranches().genMass.c_str(), &gen_mass);
    tree->SetBranchAddress(getTreeBranches().genPt.c_str(), &gen_pt);

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
    ParticleCollection genParticles;
    //This seems problematic
    // std::cout << gen_size << std::endl; 
    // std::cout << gen_size << std::endl; 
        
    for(int i = 0; i < gen_size; i++) {
        // Lorentz four-vector
        /* if(i == 0) {
            std::cout << elec_pt[i] << " " << elec_eta[i] << " " <<
             elec_phi[i] << " " << elec_mass[i] << std::endl;
        } */
        int charge = 1;
        if(gen_pid[i] < 0)
        {
            charge = -1;
        }
        if(gen_pid[i] == 21 || gen_pid[i] == 22)
        {
            charge = 0;
        }
        Particle::Type type = Particle::Type::None;
        if(std::abs(gen_pid[i]) == 11)
        {
            type = Particle::Type::Electron;
        }
        else if(std::abs(gen_pid[i]) == 13)
        {
            type = Particle::Type::Muon;
        }
        genParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i], 
            gen_eta[i], gen_phi[i], gen_mass[i])), charge, type));        
    }
    
    return genParticles;
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

ParticleCollection TreeEventFile::getRecoJets() const
{
    ParticleCollection recoParticles;
    for(int i = 0; i < jet_size; i++) {
        // Lorentz four-vector
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], 
            jet_phi[i], jet_mass[i]), 0, Particle::Type::Jet));        
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

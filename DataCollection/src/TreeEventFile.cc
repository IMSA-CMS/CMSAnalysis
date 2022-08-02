#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "CMSAnalysis/DataCollection/interface/TreeEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <iostream>
#include <vector>
#include <fstream>

TreeEventFile::TreeEventFile(TFile *ifile, const BranchNames& branchNames) : 
    EventFile(ifile), 
    // These objects take the value of the branch. 
    // They change as the treereader advances to the next event. 
    // The size variables are of type int_t and all the others are of type int_t[] or float_t[].
    elec_size(treeReader, branchNames.elecSize.c_str()),
    elec_eta(treeReader, branchNames.elecEta.c_str()),
    elec_phi(treeReader, branchNames.elecPhi.c_str()),
    elec_mass(treeReader, branchNames.elecMass.c_str()),
    elec_charge(treeReader, branchNames.elecCharge.c_str()),
    elec_pt(treeReader, branchNames.elecPT.c_str()),
    elec_reliso(treeReader, branchNames.elecRelIso.c_str()),
    muon_size(treeReader, branchNames.muonSize.c_str()),
    muon_eta(treeReader, branchNames.muonEta.c_str()),
    muon_phi(treeReader, branchNames.muonPhi.c_str()),
    muon_mass(treeReader, branchNames.muonMass.c_str()),
    muon_charge(treeReader, branchNames.muonCharge.c_str()),
    muon_pt(treeReader, branchNames.muonPT.c_str()),
    muon_reliso(treeReader, branchNames.muonRelIso.c_str()),
    met_size(treeReader, branchNames.metSize.c_str()),
    met_phi(treeReader, branchNames.metPhi.c_str()),
    met_pt(treeReader, branchNames.metPT.c_str()),
    jet_size(treeReader, branchNames.jetSize.c_str()),
    jet_eta(treeReader, branchNames.jetEta.c_str()),
    jet_phi(treeReader, branchNames.jetPhi.c_str()),
    jet_mass(treeReader, branchNames.jetMass.c_str()),
    jet_pt(treeReader, branchNames.jetPT.c_str()),
    //bJet(treeReader, branchNames.bJet.c_str()),
    gen_size(treeReader, branchNames.genSize.c_str()),
    gen_pid(treeReader, branchNames.genPid.c_str()),
    gen_status(treeReader, branchNames.genStatus.c_str()),
    gen_eta(treeReader, branchNames.genEta.c_str()),
    gen_phi(treeReader, branchNames.genPhi.c_str()),
    gen_mass(treeReader, branchNames.genMass.c_str()),
    gen_pt(treeReader, branchNames.genPt.c_str()),
    gen_d1(treeReader, branchNames.gend1.c_str()),
    gen_d2(treeReader, branchNames.gend2.c_str()),
    gen_m1(treeReader, branchNames.genm1.c_str()),
    gen_m2(treeReader, branchNames.genm2.c_str()),
    elec_idpass(treeReader, branchNames.elecIdpass.c_str()),
    muon_idpass(treeReader, branchNames.muonIdpass.c_str())
{

}

void TreeEventFile::initialize()
{
    tree = getFile()->Get<TTree>(getTreeName().c_str());
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

void TreeEventFile::nextEvent()
{
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
}

// GenEventInfoProduct TreeEventFile::getGenInfo() const
// {
//     throw std::runtime_error("Not yet implemented: getGenInfo()");

//     /*
//     edm::Handle<GenEventInfoProduct> genInfo;
//     event->getByLabel(std::string("generator"), genInfo);
//     return *genInfo;
//     */
// }

ParticleCollection<GenSimParticle> TreeEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    for (int i = 0; i < *gen_size; i++)
    {
        if (gen_status[i] != 1)
        {
            continue;
        }
        int charge = -1;
        if (gen_pid[i] < 0)
        {
            charge = 1;
        }
        if (gen_pid[i] == 21 || gen_pid[i] == 22)
        {
            charge = 0;
        }
        genParticles.addParticle(
            Particle(
                reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i],gen_eta[i], gen_phi[i], gen_mass[i])),
                charge, 
                Particle::identifyType(gen_pid[i]),gen_pid[i],gen_status[i],gen_m1[i],gen_m2[i],gen_d1[i],gen_d2[i],
                0)); //not sure if relIso, last parameter, should be set to 0
    }
    return genParticles;
}

ParticleCollection<Particle> TreeEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (int i = 0; i < *elec_size; i++)
    {
        if (elec_idpass[i] & 7)
        {
            int charge = elec_charge[i];
            recoParticles.addParticle(
                Particle(
                    reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elec_pt[i],elec_eta[i], elec_phi[i], elec_mass[i])),
                    charge, 
                    Particle::Type::Electron, 
                    elec_reliso[i]));
        }
    }
    for (int i = 0; i < *muon_size; i++)
    {
        if (muon_idpass[i] & 7)
        {
            int charge = muon_charge[i];
            recoParticles.addParticle(
                Particle(
                    reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muon_pt[i],muon_eta[i], muon_phi[i], muon_mass[i])),
                    charge, 
                    Particle::Type::Muon, 
                    muon_reliso[i]));
        }
    }
    return recoParticles;
}

ParticleCollection<Particle> TreeEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(int i = 0; i < *jet_size; i++) {
        //if(bJet[i] > 0){
            recoParticles.addParticle(
                Particle(reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]), 
                0, 
                Particle::Type::Jet));    
        //}    
    }
    return recoParticles;
}

double TreeEventFile::getMET() const
{
    return static_cast<double>(met_pt[0]);
}

bool TreeEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

std::vector<bool> TreeEventFile::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

std::vector<std::string> TreeEventFile::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

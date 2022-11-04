#include "CMSAnalysis/DataCollection/interface/DelphesEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


DelphesEventFile::DelphesEventFile(TFile* ifile) : 
    TreeEventFile(ifile, getTreeBranches()) 
{
    initialize();
}

std::string DelphesEventFile::getTreeName() 
{
    return "myana/mytree";
}

ParticleCollection<GenSimParticle> DelphesEventFile::getGenSimParticles() const
{
        ParticleCollection<GenSimParticle> genParticles;
        for (int i = 0; i < *gen_size; i++)
        {
        if (gen_status[i] != 1)
            continue;

        int charge = -1;
        if (gen_pid[i] < 0)
        {
            charge = 1;
        }
        if (gen_pid[i] == 21 || gen_pid[i] == 22)
        {
            charge = 0;
        }
                //std::cout << "Tree Event File: about to add particles\n";
        genParticles.addParticle(Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i],
                                                                        gen_eta[i], gen_phi[i], gen_mass[i])),
            charge, Particle::identifyType(gen_pid[i]),gen_pid[i],gen_status[i],gen_m1[i],gen_m2[i],gen_d1[i],gen_d2[i],0)); //not sure if relIso, last parameter, should be set to 0 
        }
    return genParticles;
}

DelphesEventFile::BranchNames DelphesEventFile::getTreeBranches() {
    BranchNames delphesBranches;

    delphesBranches.elecSize = "elec_size";
    delphesBranches.elecEta = "elec_eta";
    delphesBranches.elecPhi = "elec_phi";
    delphesBranches.elecPT = "elec_pt";
    delphesBranches.elecMass = "elec_mass";
    delphesBranches.elecCharge = "elec_charge";
    delphesBranches.elecIdpass = "elec_idpass";
    delphesBranches.elecRelIso = "elec_reliso";

    delphesBranches.muonSize = "muon_size";
    delphesBranches.muonEta = "muon_eta";
    delphesBranches.muonPhi = "muon_phi";
    delphesBranches.muonPT = "muon_pt";
    delphesBranches.muonMass = "muon_mass";
    delphesBranches.muonCharge = "muon_charge";
    delphesBranches.muonIdpass = "muon_idpass";
    delphesBranches.muonRelIso = "muon_reliso";
    
    //delphesBranches.metSize = "metpuppi_size"; // useless variable
    delphesBranches.metPT = "metpuppi_pt";
    delphesBranches.metPhi = "metpuppi_phi";

    delphesBranches.jetSize = "jetpuppi_size";
    delphesBranches.jetEta = "jetpuppi_eta";
    delphesBranches.jetPhi = "jetpuppi_phi";
    delphesBranches.jetPT = "jetpuppi_pt";
    delphesBranches.jetMass = "jetpuppi_mass";

    delphesBranches.genSize = "genpart_size";
    delphesBranches.genPid = "genpart_pid";
    delphesBranches.genStatus = "genpart_status";
    delphesBranches.genEta = "genpart_eta";
    delphesBranches.genPhi = "genpart_phi";
    delphesBranches.genMass = "genpart_mass";
    delphesBranches.genPt = "genpart_pt";
    delphesBranches.gend1 = "genpart_d1";
    delphesBranches.gend2 = "genpart_d2";
    delphesBranches.genm1 = "genpart_m1";
    delphesBranches.genm2 = "genpart_m2";

    return delphesBranches;
}
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

std::string DelphesEventFile::getTreeName() {
    return "myana/mytree";
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
    
    delphesBranches.metSize = "metpuppi_size";
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
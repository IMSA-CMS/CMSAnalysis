// MORE CHANGES NEEDED FOR IT TO WORK
// SO FAR I JUST COPIED FROM MINIAODEVENTLOADER
// Andy, [10/06/21]

#include "CIAnalysis/CIStudies/interface/DelphesEventFile.hh"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


DelphesEventFile::DelphesEventFile(TFile* ifile) : TreeEventFile(ifile) 
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

    delphesBranches.muonSize = "muon_size";
    delphesBranches.muonEta = "muon_eta";
    delphesBranches.muonPhi = "muon_phi";
    delphesBranches.muonPT = "muon_pt";
    delphesBranches.muonMass = "muon_mass";
    delphesBranches.muonCharge = "muon_charge";
    
    delphesBranches.metSize = "metpuppi_size";
    delphesBranches.metPT = "metpuppi_pt";
    delphesBranches.metPhi = "metpuppi_phi";

    delphesBranches.genSize = "genpart_size";
    delphesBranches.genPid = "genpart_pid";
    delphesBranches.genStatus = "genpart_status";
    delphesBranches.genEta = "genpart_eta";
    delphesBranches.genPhi = "genpart_phi";
    delphesBranches.genMass = "genpart_mass";
    delphesBranches.genPt = "genpart_pt";

    return delphesBranches;
}

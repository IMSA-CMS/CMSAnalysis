// MORE CHANGES NEEDED FOR IT TO WORK
// SO FAR I JUST COPIED FROM MINIAODEVENTLOADER
// Andy, [10/06/21]

#include "CIAnalysis/CIStudies/interface/DelphesEventLoader.hh"
#include "CIAnalysis/CIStudies/interface/InputModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


std::string DelphesEventLoader::getTreeName() {
    return "myana/mytree";
}

DelphesEventLoader::BranchNames DelphesEventLoader::getTreeBranches() {
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

    return delphesBranches;
}

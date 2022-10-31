#include "CMSAnalysis/DataCollection/interface/StrippedEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


StrippedEventFile::StrippedEventFile(TFile* ifile) : 
    TreeEventFile(ifile, getTreeBranches()) 
{
    initialize();
}

std::string StrippedEventFile::getTreeName() {
    return "stripped";
}

StrippedEventFile::BranchNames StrippedEventFile::getTreeBranches() {
    BranchNames strippedBranches;

    strippedBranches.elecSize = "elecSize";
    strippedBranches.elecEta = "elecEta";
    strippedBranches.elecPhi = "elecPhi";
    strippedBranches.elecPT = "elecPt";
    strippedBranches.elecMass = "elecMass";
    strippedBranches.elecCharge = "elecCharge";
    strippedBranches.elecIdpass = "elecIDPass";
    strippedBranches.elecRelIso = "elecReliso";

    strippedBranches.muonSize = "muonSize";
    strippedBranches.muonEta = "muonEta";
    strippedBranches.muonPhi = "muonPhi";
    strippedBranches.muonPT = "muonPt";
    strippedBranches.muonMass = "muonMass";
    strippedBranches.muonCharge = "muonCharge";
    strippedBranches.muonIdpass = "muonIDPass";
    strippedBranches.muonRelIso = "muonReliso";
    
    strippedBranches.metPT = "metPt";
    strippedBranches.metPhi = "metPhi";

    strippedBranches.jetSize = "jetSize";
    strippedBranches.jetEta = "jetEta";
    strippedBranches.jetPhi = "jetPhi";
    strippedBranches.jetPT = "jetPt";
    strippedBranches.jetMass = "jetMass";

    strippedBranches.genSize = "genSize";
    strippedBranches.genPid = "genPID";
    strippedBranches.genStatus = "genStatus";
    strippedBranches.genEta = "genEta";
    strippedBranches.genPhi = "genPhi";
    strippedBranches.genMass = "genMass";
    strippedBranches.genPt = "genPt";
    strippedBranches.gend1 = "genD1";
    strippedBranches.gend2 = "genD2";
    strippedBranches.genm1 = "genM1";
    strippedBranches.genm2 = "genM2";

    return strippedBranches;
}


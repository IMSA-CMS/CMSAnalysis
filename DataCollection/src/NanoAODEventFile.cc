#include "CMSAnalysis/DataCollection/interface/NanoAODEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


NanoAODEventFile::NanoAODEventFile(TFile* ifile) : TreeEventFile(ifile, getTreeBranches()) 
{
    initialize();
}

std::string NanoAODEventFile::getTreeName() {
    return "Events";
}

NanoAODEventFile::BranchNames NanoAODEventFile::getTreeBranches() {
    BranchNames nanoAODBranches;

    nanoAODBranches.elecSize = "nElectron";
    nanoAODBranches.elecEta = "Electron_eta";
    nanoAODBranches.elecPhi = "Electron_phi";
    nanoAODBranches.elecPT = "Electron_pt";
    nanoAODBranches.elecMass = "Electron_mass";
    nanoAODBranches.elecCharge = "Electron_charge";
    nanoAODBranches.elecIdpass = "Electron_cutBased";
    nanoAODBranches.elecRelIso = "Electron_miniPFRelIso_all";

    nanoAODBranches.muonSize = "nMuon";
    nanoAODBranches.muonEta = "Muon_eta";
    nanoAODBranches.muonPhi = "Muon_phi";
    nanoAODBranches.muonPT = "Muon_pt";
    nanoAODBranches.muonMass = "Muon_mass";
    nanoAODBranches.muonCharge = "Muon_charge";
    nanoAODBranches.muonIdpass = "Muon_looseId";
    nanoAODBranches.muonRelIso = "Muon_miniPFRelIso_all";
    
    //nanoAODBranches.metSize = "MET_sumEt"; // useless variable
    nanoAODBranches.metPT = "PuppiMET_pt";
    nanoAODBranches.metPhi = "MET_phi";

    nanoAODBranches.jetSize = "nJet";
    nanoAODBranches.jetEta = "Jet_eta";
    nanoAODBranches.jetPhi = "Jet_phi";
    nanoAODBranches.jetPT = "Jet_pt";
    nanoAODBranches.jetMass = "Jet_mass";

    nanoAODBranches.genSize = "nGenPart";
    nanoAODBranches.genPid = "GenPart_pdgId";
    nanoAODBranches.genStatus = "GenPart_status";
    nanoAODBranches.genEta = "GenPart_eta";
    nanoAODBranches.genPhi = "GenPart_phi";
    nanoAODBranches.genMass = "GenPart_mass";
    nanoAODBranches.genPt = "GenPart_pt";
    nanoAODBranches.gend1 = "Generator_id1"; // missing proper branch address
    nanoAODBranches.gend2 = "Generator_id2"; // missing proper branch address
    nanoAODBranches.genm1 = "GenPart_genPartIdxMother";
    nanoAODBranches.genm2 = "GenVisTau_genPartIdxMother"; // missing variable, should be fine

    return nanoAODBranches;
}

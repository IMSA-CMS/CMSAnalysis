// MORE CHANGES NEEDED FOR IT TO WORK
// SO FAR I JUST COPIED FROM MINIAODEVENTLOADER
// Andy, [10/06/21]

#include "TTree.h"
#include "CMSAnalysis/DataCollection/interface/TreeEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <iostream>
#include <vector>
#include <fstream>

TreeEventFile::TreeEventFile(TFile *ifile) : EventFile(ifile) {}
int otherCounter = 0;
int otherotherCounter =1;
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
    tree->SetBranchAddress(getTreeBranches().elecRelIso.c_str(), &elec_reliso);

    tree->SetBranchAddress(getTreeBranches().muonSize.c_str(), &muon_size);
    tree->SetBranchAddress(getTreeBranches().muonEta.c_str(), &muon_eta);
    tree->SetBranchAddress(getTreeBranches().muonPhi.c_str(), &muon_phi);
    tree->SetBranchAddress(getTreeBranches().muonMass.c_str(), &muon_mass);
    tree->SetBranchAddress(getTreeBranches().muonCharge.c_str(), &muon_charge);
    tree->SetBranchAddress(getTreeBranches().muonPT.c_str(), &muon_pt);
    tree->SetBranchAddress(getTreeBranches().muonRelIso.c_str(), &muon_reliso);

    tree->SetBranchAddress(getTreeBranches().metSize.c_str(), &met_size);
    tree->SetBranchAddress(getTreeBranches().metPhi.c_str(), &met_phi);
    tree->SetBranchAddress(getTreeBranches().metPT.c_str(), &met_pt);

    tree->SetBranchAddress(getTreeBranches().jetSize.c_str(), &jet_size);
    tree->SetBranchAddress(getTreeBranches().jetEta.c_str(), &jet_eta);
    tree->SetBranchAddress(getTreeBranches().jetPhi.c_str(), &jet_phi);
    tree->SetBranchAddress(getTreeBranches().jetMass.c_str(), &jet_mass);
    tree->SetBranchAddress(getTreeBranches().jetPT.c_str(), &jet_pt);
    //tree->SetBranchAddress(getTreeBranches().bJet.c_str(), &bJet);

    tree->SetBranchAddress(getTreeBranches().genSize.c_str(), &gen_size);
    tree->SetBranchAddress(getTreeBranches().genPid.c_str(), &gen_pid);
    tree->SetBranchAddress(getTreeBranches().genStatus.c_str(), &gen_status);
    tree->SetBranchAddress(getTreeBranches().genEta.c_str(), &gen_eta);
    tree->SetBranchAddress(getTreeBranches().genPhi.c_str(), &gen_phi);
    tree->SetBranchAddress(getTreeBranches().genMass.c_str(), &gen_mass);
    tree->SetBranchAddress(getTreeBranches().genPt.c_str(), &gen_pt);
    tree->SetBranchAddress(getTreeBranches().gend1.c_str(), &gen_d1);
    tree->SetBranchAddress(getTreeBranches().gend2.c_str(), &gen_d2);
    tree->SetBranchAddress(getTreeBranches().genm1.c_str(), &gen_m1);
    tree->SetBranchAddress(getTreeBranches().genm2.c_str(), &gen_m2);

    tree->SetBranchAddress(getTreeBranches().elecIdpass.c_str(), &elec_idpass);
    tree->SetBranchAddress(getTreeBranches().muonIdpass.c_str(), &muon_idpass);

    //tree->Show();
    //nextEvent();
    counter = 0;
    tree->GetEntry(counter);
    ++(counter);

    // // Now loop through by index to collect everything??
    // Int_t nentries = (Int_t)t1->GetEntries();
    // for(int i = 0; i < nentries; i++) {
    //     nextEvent();
    // }
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

ParticleCollection<GenSimParticle> TreeEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    // This seems problematic
    //  std::cout << gen_size << std::endl;
    //  std::cout << gen_size << std::endl;
    //int leptonCount = 0;
    //int element = 1;
    //std::vector<std::vector<int>> leptonInfo;
    // std::cout << "--------------------------------------------------------" << std::endl;
    // std::cout << "EVENT #" << (otherCounter + 1)<<std::endl;
    // std::cout << "--------------------------------------------------------" << std::endl;

    // std::cout << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
    // << std::setw(10) << "| status"
    // << std::setw(motherColumnWidth) << "| mothers"
    // << std::setw(daughterColumnWidth) << "| daughters"
    // << std::setw(15) << "| pT"
    // << std::setw(15) << "| Eta"
    // << std::setw(15) << "| Phi"
    // //<< std::setw(15) << "| E"
    // << std::setw(5) << "| mass\n";
    for (int i = 0; i < gen_size; i++)
    {
        //std::cout<< std::setw(8) << i+1 << "| " << std::setw(9) << gen_pid[i] << "| " << std::setw(8) << gen_status[i] << "| ";

        // fix formatMotherParticles function
        // Print mothers
        //std::cout << std::setw(motherColumnWidth - 2) << "0" << "| ";
        //my_file << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";

        // Print daughters
        //formatDaughterParticles(part, particleGroup)
        //std::cout << std::setw(daughterColumnWidth - 2) << "A"<< "| ";

        // Particle properties
        //std::cout << std::setw(13) << gen_m1[i]<<""<<gen_m2[i]<<"| "<<std::setw(13)<< gen_d1[i]<<""<<gen_d2[i]<<"| "<<std::setw(13)<<gen_pt[i] << "| " << std::setw(13) << gen_eta[i] << "| " << std::setw(13) << gen_phi[i] << "| " << std::setw(13) << gen_mass[i] << "\n";
        //std::cout<<"elec_idpass "<<elec_idpass[i]<<std::endl;
        // Lorentz four-vector
        /* if(i == 0) {
            std::cout << elec_pt[i] << " " << elec_eta[i] << " " <<
             elec_phi[i] << " " << elec_mass[i] << std::endl;
        } */

        // This is specific to leptons
        // NEEDS TO BE CHANGED TO BE MORE ROBUST

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
            //std::cout << "Tree Event File:" << genParticles.getNumParticles() << "\n";
        // if((gen_pid[i]==13 || gen_pid[i]==-13 ||gen_pid[i]==11 || gen_pid[i]==-11) && gen_pt[i]>=50 && gen_status[i] == 1){
        //     leptonCount++;
        //     std::vector<int> lepton;
        //     lepton.emplace_back(i+1);
        //     lepton.emplace_back(gen_pt[i]);
        //     lepton.emplace_back(gen_eta[i]);
        //     lepton.emplace_back(gen_mass[i]);
        //     lepton.emplace_back(gen_pid[i]);
        //     leptonInfo.emplace_back(lepton);
        // }   
    }
    // bool fourLepton = false;
    // int totalPt = 0;
    // //std::cout<<leptonCount<<std::endl;
    // if (leptonInfo.size() ==4){
    //     std::cout<<std::endl;
    //     for (size_t i = 0; i < 4; i++)
    //     {
    //         std::cout<<"ID"<<leptonInfo[i][0]<<" "<<leptonInfo[i][1]<<" "<<leptonInfo[i][2] <<" "<<leptonInfo[i][3]<<leptonInfo[i][4]<<std::endl;
    //         totalPt += leptonInfo[i][1];
    //     }
    //     std::cout<<std::endl;
    //     fourLepton = true;
    // }
    // std::ofstream my_file;
    // my_file.open("EventDumpAndyWWZ3000#2.txt", std::ios::app);
    // if(fourLepton && totalPt >= 3000){
    //     my_file << "--------------------------------------------------------" << std::endl;
    //     my_file << "EVENT #" << (otherCounter + 1)<<" "<< otherotherCounter<<"ab" <<std::endl;
    //     my_file << "--------------------------------------------------------" << std::endl;
    //     my_file << std::left << std::setw(8) << "element" << std::setw(11) << "| pdfId"
    //         << std::setw(10) << "| status"
    //         << std::setw(motherColumnWidth) << "| mothers"
    //         << std::setw(daughterColumnWidth) << "| daughters"
    //         << std::setw(15) << "| pT"
    //         << std::setw(15) << "| Eta"
    //         << std::setw(15) << "| Phi"
    //         //<< std::setw(15) << "| E"
    //         << std::setw(5) << "| mass\n";
    //     for (int i = 0; i < gen_size; i++)
    //     {
    //         my_file<< std::setw(8) << i+1 << "| " << std::setw(9) << gen_pid[i] << "| " << std::setw(8) << gen_status[i] << "| ";
    //         my_file << std::setw(13) << gen_m1[i]<<""<<gen_m2[i]<<"| "<<std::setw(13)<< gen_d1[i]<<""<<gen_d2[i]<<"| "<<std::setw(13)<<gen_pt[i] << "| " << std::setw(13) << gen_eta[i] << "| " << std::setw(13) << gen_phi[i] << "| " << std::setw(13) << gen_mass[i] << "\n";
    //     }
    //     my_file<<std::endl;
    //     for (size_t i = 0; i < 4; i++)
    //     {
    //         my_file<<leptonInfo[i][0]<<" "<<leptonInfo[i][1]<<" "<<leptonInfo[i][2] <<" "<<leptonInfo[i][3]<<std::endl;
    //     }
    //     my_file<<std::endl;
    //     otherotherCounter++;
    // }
    // my_file.close();
    // fourLepton = false;
    // otherCounter++;
    //std::cout<< "Size of GenParticles "<< genParticles.getNumParticles()<<std::endl;
    return genParticles;
}

ParticleCollection<Particle> TreeEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    //  std::cout << elec_size << std::endl;
    //  std::cout << muon_size << std::endl;

    for (int i = 0; i < elec_size; i++)
    {
        
        //std::cout<<"elec_idpass "<<elec_idpass[i]<<std::endl;
        int charge = elec_charge[i];
        
        Particle::SelectionFit fit;
        if (elec_idpass[i] & 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (elec_idpass[i] & 2) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (elec_idpass[i] & 1) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }


        // Lorentz four-vector
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elec_pt[i],
                                                                        elec_eta[i], elec_phi[i], elec_mass[i])),
            charge, Particle::Type::Electron, elec_reliso[i], fit));
        
    }

    for (int i = 0; i < muon_size; i++)
    {
        
        int charge = muon_charge[i];
        
        Particle::SelectionFit fit;
        if (muon_idpass[i] & 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (muon_idpass[i] & 2) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (muon_idpass[i] & 1) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }
        // Lorentz four-vector
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muon_pt[i],
                                                                        muon_eta[i], muon_phi[i], muon_mass[i])),
            charge, Particle::Type::Muon, muon_reliso[i], fit));
        
    }
    //std::cout<< "Size of RecoParticles "<< recoParticles.getNumParticles()<<std::endl;
    return recoParticles;
}

ParticleCollection<Particle> TreeEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(int i = 0; i < jet_size; i++) {
        if(bJet[i] > 0){
        // Lorentz four-vector
            recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], 
            jet_phi[i], jet_mass[i]), 0, Particle::Type::Jet));    
        }    
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

std::vector<bool> TreeEventFile::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

std::vector<std::string> TreeEventFile::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

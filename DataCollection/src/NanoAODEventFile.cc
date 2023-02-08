#include "CMSAnalysis/DataCollection/interface/NanoAODEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<bool> NanoAODEventFile::getTriggerResults(std::string subProcess) const
{
    // determines whether it passes the trigger"s criteria or not
    std::vector<bool> v_results = {};

    for(auto& trigger : triggers)
    {
        v_results.push_back(*(trigger.second));
    }

    return v_results;
}

std::vector<std::string> NanoAODEventFile::getTriggerNames(std::string subProcess) const
{
    std::vector<std::string> v_names = {};

    for(auto& trigger : triggers)
    {
        v_names.push_back(trigger.first);
    }

    return v_names;
}

bool NanoAODEventFile::checkTrigger(std::string triggerName, std::string subProcess) const
{
    return *(triggers.find(triggerName)->second);
}

NanoAODEventFile::NanoAODEventFile(TFile *ifile) : 
    EventFile(ifile)
    // // These objects take the value of the branch. 
    // // They change as the treereader advances to the next event. 
    // elec_size(treeReader, "nElectron"),
    // elec_eta(treeReader, "Electron_eta"),
    // elec_phi(treeReader, "Electron_phi"),
    // elec_mass(treeReader, "Electron_mass"),
    // elec_charge(treeReader, "Electron_charge"),
    // elec_pt(treeReader, "Electron_pt"),
    // elec_reliso(treeReader, "Electron_miniPFRelIso_all"),
    // elec_dxy(treeReader, "Electron_dxy"),
    // elec_dz(treeReader, "Electron_dz"),
    // muon_size(treeReader, "nMuon"),
    // muon_eta(treeReader, "Muon_eta"),
    // muon_phi(treeReader, "Muon_phi"),
    // muon_mass(treeReader, "Muon_mass"),
    // muon_charge(treeReader, "Muon_charge"),
    // muon_pt(treeReader, "Muon_pt"),
    // muon_reliso(treeReader, "Muon_miniPFRelIso_all"),
    // muon_dxy(treeReader, "Muon_dxy"),
    // muon_dz(treeReader, "Muon_dz"),
    // photon_size(treeReader, "nPhoton"),
    // photon_eta(treeReader, "Photon_eta"),
    // photon_phi(treeReader, "Photon_phi"),
    // photon_pt(treeReader, "Photon_pt"),
    // met_phi(treeReader, "MET_phi"),
    // met_pt(treeReader, "MET_pt"),
    // jet_size(treeReader, "nJet"),
    // jet_eta(treeReader, "Jet_eta"),
    // jet_phi(treeReader, "Jet_phi"),
    // jet_mass(treeReader, "Jet_mass"),
    // jet_pt(treeReader, "Jet_pt"),
    // elec_idpass(treeReader, "Electron_cutBased"),
    // muon_looseid(treeReader, "Muon_looseId"),
    // muon_mediumid(treeReader, "Muon_mediumId"),
    // muon_tightid(treeReader, "Muon_tightId"),
    // event_number(treeReader, "event"),
    // jet_bTag(treeReader, "Jet_btagCMVA"),
    // gen_size(treeReader, "nGenPart"),
    // gen_pid(treeReader, "GenPart_pdgId"),
    // gen_status(treeReader, "GenPart_status"),
    // gen_eta(treeReader, "GenPart_eta"),
    // gen_phi(treeReader, "GenPart_phi"),
    // gen_mass(treeReader, "GenPart_mass"),
    // gen_pt(treeReader, "GenPart_pt"),
    // gen_d1(treeReader, "Generator_id1"),
    // gen_d2(treeReader, "Generator_id2"),
    // gen_m1(treeReader, "GenPart_genPartIdxMother"),
    // gen_m2(treeReader, "GenVisTau_genPartIdxMother"),
    // gen_pileup(treeReader, "Pileup_nTrueInt")
    {
    //initializing variables from header file
    std::vector<std::shared_ptr<TreeVariableBase>> treeVariables = {
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("elec_size", "nElectron"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_eta", "Electron_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_phi", "Electron_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_mass", "Electron_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("elec_charge", "Electron_charge"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_pt", "Electron_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_reliso", "Electron_miniPFRelIso_all"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dxy", "Electron_dxy"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dz", "Electron_dz"),
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("muon_size", "nMuon"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_eta", "Muon_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_phi", "Muon_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_mass", "Muon_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("muon_charge", "Muon_charge"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_pt", "Muon_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_reliso", "Muon_miniPFRelIso_all"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_dxy", "Muon_dxy"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_dz", "Muon_dz"),
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("photon_size", "nPhoton"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_eta", "Photon_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_phi", "Photon_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_pt", "Photon_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("met_phi", "MET_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("met_pt", "MET_pt"),
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("jet_size", "nJet"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_eta", "Jet_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_phi", "Jet_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_mass", "Jet_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_pt", "Jet_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("elec_idpass", "Electron_cutBased"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_looseid", "Muon_looseId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_mediumid", "Muon_mediumId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_tightid", "Muon_tightId"),
		std::make_shared<TreeVariable<TTreeReaderValue<ULong64_t>>>("event_number", "event"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_bTag", "Jet_btagCMVA"),
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("gen_size", "nGenPart"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_pid", "GenPart_pdgId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_status", "GenPart_status"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_eta", "GenPart_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_phi", "GenPart_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_mass", "GenPart_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_pt", "GenPart_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_d1", "Generator_id1"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_d2", "Generator_id2"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_m1", "GenPart_genPartIdxMother"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_m2", "GenVisTau_genPartIdxMother"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_pileup", "Pileup_nTrueInt")
    };

    tree = getFile()->Get<TTree>("Events");

    for(auto& var : treeVariables)
    {
        // std::cout << "Checking " << var->getBranchName() << std::endl;
        if(tree->GetBranch(var->getBranchName().c_str()))
        {
            // std::cout << "Adding " << var->getBranchName() << std::endl;
            variables.emplace(var->getName(), var->makeReader(treeReader));
        }
    }

    //initializing triggers from header file
    std::ifstream triggerNameFile("betterValidTriggers.txt");

    if(!triggerNameFile)
    {
        std::string nameoftrigger;

        while(getline(triggerNameFile, nameoftrigger))
        {
            if(tree->GetBranch(nameoftrigger.c_str()))
            {
                TTreeReaderValue<Bool_t> intermediate(treeReader, nameoftrigger.c_str());
                triggers.emplace(nameoftrigger, intermediate);
            }
        }
    }    
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

void NanoAODEventFile::nextEvent()
{
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
    // std::cerr << gen_size.IsValid() << std::endl;
    // std::cerr << "event number" << *event_number << std::endl;

    // if(variables["gen_size"]->IsValid())
    // {
    //     genSimParticles.clear();
    //     genSimParticles.reserve(*gen_size);

    //     for (unsigned i = 0; i < *gen_size; i++)
    //     {
    //         int charge = -1;
    //         if (gen_pid[i] < 0)
    //         {
    //             charge = 1;
    //         }
    //         if (gen_pid[i] == 21 || gen_pid[i] == 22)
    //         {
    //             charge = 0;
    //         }

    //         std::vector<const GenSimParticle *> daughterCollectionVector{&genSimParticles[gen_d1[i]], &genSimParticles[gen_d2[i]]};

    //         genSimParticles.push_back(GenSimParticle(
    //             reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i],
    //                                                                         gen_eta[i], gen_phi[i], gen_mass[i])),
    //             charge, Particle::identifyType(gen_pid[i]), &genSimParticles[gen_m1[i]],
    //             daughterCollectionVector, gen_status[i]));
    //     }
    // }
}

ParticleCollection<GenSimParticle> NanoAODEventFile::getGenSimParticles() const
{
    return genSimParticles;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;

    for (UInt_t i = 0; i < getVariable<UInt_t>("elec_size"); i++)
    {
        int charge = getArrayElement<Int_t>("elec_charge", i);
        
        Particle::SelectionFit fit;
        if (getArrayElement<Int_t>("elec_idpass", i) == 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (getArrayElement<Int_t>("elec_idpass", i) == 3) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (getArrayElement<Int_t>("elec_idpass", i) == 2) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }


        // Lorentz four-vector
        auto particle = Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("elec_pt", i),
                                                                        getArrayElement<Float_t>("elec_eta", i), getArrayElement<Float_t>("elec_phi", i), getArrayElement<Float_t>("elec_mass", i))),
            charge, ParticleType::electron(), fit);
        particle.addInfo("Isolation", getArrayElement<Float_t>("elec_reliso", i));
        particle.addInfo("dxy", getArrayElement<Float_t>("elec_dxy", i));
        particle.addInfo("dz", getArrayElement<Float_t>("elec_dz", i));
        recoParticles.addParticle(particle);
    }

    for (UInt_t i = 0; i < getVariable<UInt_t>("muon_size"); i++)
    {
        int charge = getArrayElement<Int_t>("muon_charge", i);
        
        Particle::SelectionFit fit;
        if (getArrayElement<Bool_t>("muon_tightid", i)) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (getArrayElement<Bool_t>("muon_mediumid", i)) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (getArrayElement<Bool_t>("muon_looseid", i)) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }

        // Lorentz four-vector
        auto particle = Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("muon_pt", i),
                                                                        getArrayElement<Float_t>("muon_eta", i), getArrayElement<Float_t>("muon_phi", i), getArrayElement<Float_t>("muon_mass", i))),
            charge, ParticleType::muon(), fit);
        particle.addInfo("Isolation", getArrayElement<Float_t>("muon_reliso", i));
        particle.addInfo("dxy", getArrayElement<Float_t>("muon_dxy", i));
        particle.addInfo("dz", getArrayElement<Float_t>("muon_dz", i));
        recoParticles.addParticle(particle);

    for (UInt_t i = 0; i < getVariable<UInt_t>("photon_size"); i++)
    {
        Particle::SelectionFit fit;

            auto particle = Particle(
                reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("photon_pt", i),
                                                                        getArrayElement<Float_t>("photon_eta", i), getArrayElement<Float_t>("photon_phi", i), 0)),
            0, ParticleType::photon(), fit);
            recoParticles.addParticle(particle);
        }
    }
    return recoParticles;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < getVariable<UInt_t>("jet_size"); i++) {
        //if(bJet[i] > 0){
            recoParticles.addParticle(
                Particle(reco::Candidate::LorentzVector(getArrayElement<Float_t>("jet_pt", i), getArrayElement<Float_t>("jet_eta", i), getArrayElement<Float_t>("jet_phi", i), getArrayElement<Float_t>("jet_mass", i)), 
                0, 
                ParticleType::jet()));    
        //}    
    }
    return recoParticles;
}

double NanoAODEventFile::getMET() const
{
    return static_cast<double>(getArrayElement<Float_t>("met_pt", 0));
}

int NanoAODEventFile::getNumPileUpInteractions() const
{
    return 1;
    // return static_cast<int>(*(variables["gen_pileup"])[0]);
}

bool NanoAODEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

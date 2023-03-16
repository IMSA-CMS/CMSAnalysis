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
        // std::cout << "Is this for loop working" << "\n";
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
    // add cout statement to check, it in fact does pass through this code
    // std::cout << "Does this Trigger work?" << "\n";
    return *(triggers.find(triggerName)->second);
}

NanoAODEventFile::NanoAODEventFile(TFile *ifile) : 
    EventFile(ifile)
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
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_m1", "GenPart_genPartIdxMother"),
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

    if(triggerNameFile)
    {
        std::string nameoftrigger;

        while(getline(triggerNameFile, nameoftrigger))
        {
            std::cout << nameoftrigger << "Does this work?" << "\n";
            if(tree->GetBranch(nameoftrigger.c_str()))
            {
                TTreeReaderValue<Bool_t> intermediate(treeReader, nameoftrigger.c_str());
                triggers.emplace(nameoftrigger, intermediate);
                std::cout << nameoftrigger <<"\n";
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

    if(variables.find("gen_size") != variables.end() && getVariable<UInt_t>("gen_size") > 0)
    {
        genSimParticles.clear();
        genSimParticles.reserve(getVariable<UInt_t>("gen_size") );

        //construct daughters from mothers
        int mothersIndex;
        std::vector<std::vector<Int_t>> daughtersVectors((int)getVariable<UInt_t>("gen_size"), std::vector<int>(0)); //Vector of Vectors, the first index represents the mother, the second vector has the aughter indexes        
        for (unsigned j = 0; j < getVariable<UInt_t>("gen_size"); j++)
        {
            mothersIndex = getArrayElement<Int_t>("gen_m1", j); //Starts at particle 0, then gets its mother
            if (mothersIndex < (int)getVariable<UInt_t>("gen_size") && mothersIndex > -1)
            {
                daughtersVectors[mothersIndex].push_back(j); //at daughtersVectors[mothersIndex], adds the current particle, making the current particle the daughter
                //daughersVectors[i] is a vector of the daughters of particle i
            }
        }

        for (unsigned i = 0; i < getVariable<UInt_t>("gen_size") ; i++) //Loops through every particle
        {
            std::vector<const GenSimParticle*> daughterCollectionVector{};
            for (auto index : daughtersVectors[i]) //for every particle, go throught its daughters and add add its index in genSimp ro daughter collection vector
            {
                //std::cout<< "index: " << index << "\n";
                //std::cout<< "size: " <<daughtersVectors[i].size() << "\n";

                daughterCollectionVector.push_back(&genSimParticles[index]); //probably something wrong here
             
               //gensim is cleared every time, so its always empty, so the daughter thing is pointing at garbage?, probably not

                //std::cout<< "gensimindex: " << &genSimParticles[index] << "\n";
                //std::cout<< "daughter collection index" << daughterCollectionVector[0] << "\n";                 //index and vector addresses are the same
            }
            GenSimParticle *mother = nullptr;
            if(getArrayElement<Int_t>("gen_m1", i) != -1){
                 mother = &genSimParticles[getArrayElement<Int_t>("gen_m1", i)];
            }
            genSimParticles.push_back(GenSimParticle(reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(
                getArrayElement<Float_t>("gen_pt", i),
                getArrayElement<Float_t>("gen_eta", i), 
                getArrayElement<Float_t>("gen_phi", i), 
                getArrayElement<Float_t>("gen_mass", i))), 
                getArrayElement<Int_t>("gen_pid", i),
                mother,
                daughterCollectionVector, 
                getArrayElement<Int_t>("gen_status", i)));
            
            
            if(daughterCollectionVector.size() > 0) {
                //std::cout<< "GenSimParticle " << &genSimParticles[daughtersVectors[i][0]] << "\n";
                //std::cout<< "Daughter zero: " << daughterCollectionVector[0] << "\n";
                if(&genSimParticles[daughtersVectors[i][0]] != daughterCollectionVector[0]) {
                    //std::cout << "ERROR!" << "\n";
                }

                //std::cout<< "Daughter: " << daughterCollectionVector[0]->getName(); //segfaulted
            }

        }
    }
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
    }
    for (UInt_t i = 0; i < getVariable<UInt_t>("photon_size"); i++)
    {
        Particle::SelectionFit fit;

        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("photon_pt", i),
        getArrayElement<Float_t>("photon_eta", i), getArrayElement<Float_t>("photon_phi", i), 0)),
        0, ParticleType::photon(), fit);
        recoParticles.addParticle(particle);
    }
    
    return recoParticles;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < getVariable<UInt_t>("jet_size"); i++) {
        recoParticles.addParticle(
        Particle(reco::Candidate::LorentzVector(getArrayElement<Float_t>("jet_pt", i), getArrayElement<Float_t>("jet_eta", i), getArrayElement<Float_t>("jet_phi", i), getArrayElement<Float_t>("jet_mass", i)), 
        0, 
        ParticleType::jet()));        
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
}

bool NanoAODEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}
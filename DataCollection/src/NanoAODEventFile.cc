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
    // determines whether it passes the trigger's criteria or not
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
    EventFile(ifile), 
    // These objects take the value of the branch. 
    // They change as the treereader advances to the next event. 
    elec_size(treeReader, "nElectron"),
    elec_eta(treeReader, "Electron_eta"),
    elec_phi(treeReader, "Electron_phi"),
    elec_mass(treeReader, "Electron_mass"),
    elec_charge(treeReader, "Electron_charge"),
    elec_pt(treeReader, "Electron_pt"),
    elec_reliso(treeReader, "Electron_miniPFRelIso_all"),
    elec_dxy(treeReader, "Electron_dxy"),
    elec_dz(treeReader, "Electron_dz"),
    muon_size(treeReader, "nMuon"),
    muon_eta(treeReader, "Muon_eta"),
    muon_phi(treeReader, "Muon_phi"),
    muon_mass(treeReader, "Muon_mass"),
    muon_charge(treeReader, "Muon_charge"),
    muon_pt(treeReader, "Muon_pt"),
    muon_reliso(treeReader, "Muon_miniPFRelIso_all"),
    muon_dxy(treeReader, "Muon_dxy"),
    muon_dz(treeReader, "Muon_dz"),
    //met_size(treeReader, branchNames.metSize.c_str()),
    met_phi(treeReader, "MET_phi"),
    met_pt(treeReader, "MET_pt"),
    jet_size(treeReader, "nJet"),
    jet_eta(treeReader, "Jet_eta"),
    jet_phi(treeReader, "Jet_phi"),
    jet_mass(treeReader, "Jet_mass"),
    jet_pt(treeReader, "Jet_pt"),
    //bJet(treeReader, branchNames.bJet.c_str()),
    gen_size(treeReader, "nGenPart"),
    gen_pid(treeReader, "GenPart_pdgId"),
    gen_status(treeReader, "GenPart_status"),
    gen_eta(treeReader, "GenPart_eta"),
    gen_phi(treeReader, "GenPart_phi"),
    gen_mass(treeReader, "GenPart_mass"),
    gen_pt(treeReader, "GenPart_pt"),
    gen_d1(treeReader, "Generator_id1"),
    gen_d2(treeReader, "Generator_id2"),
    gen_m1(treeReader, "GenPart_genPartIdxMother"),
    gen_m2(treeReader, "GenVisTau_genPartIdxMother"),
    gen_pileup(treeReader, "Pileup_nTrueInt"),
    elec_idpass(treeReader, "Electron_cutBased"),
    muon_idpass(treeReader, "Muon_looseId")
    {
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

    genSimParticles.clear();
    genSimParticles.reserve(*gen_size);

    for (unsigned i = 0; i < *gen_size; i++)
    {          
        int charge = -1;
        if (gen_pid[i] < 0)
        {
            charge = 1;
        }
        if (gen_pid[i] == 21 || gen_pid[i] == 22)
        {
            charge = 0;
        }

        std::vector<const GenSimParticle*> daughterCollectionVector {&genSimParticles[gen_d1[i]], &genSimParticles[gen_d2[i]]};

        genSimParticles.push_back(GenSimParticle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i],
                                                                        gen_eta[i], gen_phi[i], gen_mass[i])),
            charge, Particle::identifyType(gen_pid[i]),&genSimParticles[gen_m1[i]],
            daughterCollectionVector));         
        
    }
}

ParticleCollection<GenSimParticle> NanoAODEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> collectionVector;

    return collectionVector;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (UInt_t i = 0; i < *elec_size; i++)
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
            charge, ParticleType::electron(), elec_reliso[i], fit, elec_dxy[i], elec_dz[i]));
        
    }
    for (UInt_t i = 0; i < *muon_size; i++)
    {
        
        int charge = muon_charge[i];
        
        Particle::SelectionFit fit;
        // if (muon_idpass[i] & 4) 
        // {
        //     fit = Particle::SelectionFit::Tight;
        // } else if (muon_idpass[i] & 2) 
        // {
        //     fit = Particle::SelectionFit::Medium;
        // } else if (muon_idpass[i] & 1) 
        // {
        //     fit = Particle::SelectionFit::Loose;
        // } else {
        //     continue;
        // }

        // Lorentz four-vector
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muon_pt[i],
                                                                        muon_eta[i], muon_phi[i], muon_mass[i])),
            charge, ParticleType::muon(), muon_reliso[i], fit, muon_dxy[i], muon_dz[i]));
        
    }
    return recoParticles;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < *jet_size; i++) {
        //if(bJet[i] > 0){
            recoParticles.addParticle(
                Particle(reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]), 
                0, 
                ParticleType::jet()));    
        //}    
    }
    return recoParticles;
}

double NanoAODEventFile::getMET() const
{
    return static_cast<double>(met_pt[0]);
}

int NanoAODEventFile::getNumPileUpInteractions() const
{
    return static_cast<int>(gen_pileup[0]);
}

bool NanoAODEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

#include "CMSAnalysis/EventFiles/interface/DelphesEventFile.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"


DelphesEventFile::DelphesEventFile(TFile *ifile, std::shared_ptr<FileParams> iparams) : 
    EventFile(ifile, iparams),  
    // These objects take the value of the branch. 
    // They change as the treereader advances to the next event. 
    elec_size(treeReader, "elec_size"),
    elec_dxy(treeReader, "elec_dxy"),
    elec_dz(treeReader, "elec_dz"),
    elec_eta(treeReader, "elec_phi"),
    //Is this normal?
    elec_phi(treeReader, "elec_phi"),
    elec_mass(treeReader, "elec_mass"),
    elec_charge(treeReader, "elec_charge"),
    elec_pt(treeReader, "elec_pt"),
    elec_reliso(treeReader, "elec_reliso"),
    muon_size(treeReader, "muon_size"),
    muon_dxy(treeReader, "muon_dxy"),
    muon_dz(treeReader, "muon_dz"),
    muon_eta(treeReader, "muon_eta"),
    muon_phi(treeReader, "muon_phi"),
    muon_mass(treeReader, "muon_mass"),
    muon_charge(treeReader, "muon_charge"),
    muon_pt(treeReader, "muon_pt"),
    muon_reliso(treeReader, "muon_reliso"),
    met_phi(treeReader, "metpuppi_phi"),
    met_pt(treeReader, "metpuppi_pt"),
    jet_size(treeReader, "jetpuppi_size"),
    jet_eta(treeReader, "jetpuppi_eta"),
    jet_phi(treeReader, "jetpuppi_phi"),
    jet_mass(treeReader, "jetpuppi_mass"),
    jet_pt(treeReader, "jetpuppi_pt"),
    gen_size(treeReader, "genpart_size"),
    gen_pid(treeReader, "genpart_pid"),
    gen_status(treeReader, "genpart_status"),
    gen_eta(treeReader, "genpart_eta"),
    gen_phi(treeReader, "genpart_phi"),
    gen_mass(treeReader, "genpart_mass"),
    gen_pt(treeReader, "genpart_pt"),
    gen_d1(treeReader, "genpart_d1"),
    gen_d2(treeReader, "genpart_d2"),
    gen_m1(treeReader, "genpart_m1"),
    gen_m2(treeReader, "genpart_m2"),
    elec_idpass(treeReader, "elec_idpass"),
    muon_idpass(treeReader, "muon_idpass")
{
    tree = getFile()->Get<TTree>("myana/mytree");    
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

void DelphesEventFile::nextEvent()
{
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
}

ParticleCollection<GenSimParticle> DelphesEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    for (Int_t i = 0; i < *gen_size; i++) // iterator type must be defined by template to allow comparison with the size variable
    {
        if (gen_status[i] != 1)
        {
            continue;
        }
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(gen_pt[i], gen_eta[i], gen_phi[i], gen_mass[i])),
        gen_pid[i], gen_status[i], gen_m1[i], gen_m2[i], gen_d1[i], gen_d2[i]);
        
        //particle.addInfo("Isolation", 0); GAVIN CHANGED
        genParticles.addParticle(particle);
    }
    return genParticles;
}

ParticleCollection<Particle> DelphesEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (Int_t i = 0; i < *elec_size; i++)
    {
    
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
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elec_pt[i],elec_eta[i], elec_phi[i], elec_mass[i])),
        elec_dxy[i], elec_dz[i], charge, ParticleType::electron(), fit);

        //particle.addInfo("Isolation", elec_reliso[i]); GAVIN CHANGED
        recoParticles.addParticle(particle);
    }

    for (Int_t i = 0; i < *muon_size; i++)
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
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muon_pt[i],muon_eta[i], muon_phi[i], muon_mass[i])),
        muon_dxy[i], muon_dz[i], charge, ParticleType::muon(), fit);

        //particle.addInfo("Isolation", muon_reliso[i]); GAVIN CHANGED
        recoParticles.addParticle(particle);
    }
    return recoParticles;
}

ParticleCollection<Particle> DelphesEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(Int_t i = 0; i < *jet_size; i++) 
    {
        recoParticles.addParticle(
        Particle(reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]), 0, 0, 0,ParticleType::jet()));     
    }
    return recoParticles;
}

double DelphesEventFile::getMET() const
{
    return static_cast<double>(met_pt[0]);
}

unsigned long long DelphesEventFile::getEventIDNum() const
{
    throw std::runtime_error("DelphesEventFile has no implementation of getEventIDNum"); 
}

bool DelphesEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

std::vector<double> DelphesEventFile::getPDFWeights() const
{
    throw "Function not implemented";
}

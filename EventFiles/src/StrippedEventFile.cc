#include "CMSAnalysis/EventFiles/interface/StrippedEventFile.hh"


StrippedEventFile::StrippedEventFile(TFile* ifile, std::shared_ptr<FileParams> iparams) : 
    EventFile(ifile, iparams),
    elecSize(treeReader, "elecSize"),
    elecEta(treeReader, "elecEta"),
    elecPhi(treeReader, "elecPhi"),
    elecMass(treeReader, "elecMass"),
    elecCharge(treeReader, "elecCharge"),
    elecPt(treeReader, "elecPt"),
    elecReliso(treeReader, "elecReliso"),

    muonSize(treeReader, "muonSize"),
    muonPhi(treeReader, "muonPhi"),
    muonEta(treeReader, "muonEta"),
    muonMass(treeReader, "muonMass"),
    muonCharge(treeReader, "muonCharge"),
    muonPt(treeReader, "muonPt"),
    muonReliso(treeReader, "muonReliso"),

    metPhi(treeReader, "metPhi"),
    metPt(treeReader, "metPt"),

    jetSize(treeReader, "jetSize"),
    jetEta(treeReader, "jetEta"),
    jetPhi(treeReader, "jetPhi"),
    jetMass(treeReader, "jetMass"),
    jetPt(treeReader, "jetPt"),

    genSize(treeReader, "genSize"),
    genPID(treeReader, "genPID"),
    genStatus(treeReader, "genStatus"),
    genEta(treeReader, "genEta"),
    genPhi(treeReader, "genPhi"),
    genMass(treeReader, "genMass"),
    genPt(treeReader, "genPt"),
    genD1(treeReader, "genD1"),
    genD2(treeReader, "genD2"),
    genM1(treeReader, "genM1"),
    genM2(treeReader, "genM2"),
    elecIDPass(treeReader, "elecIDPass"),
    muonIDPass(treeReader, "muonIDPass")
{
    tree = getFile()->Get<TTree>("stripped");    
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

void StrippedEventFile::nextEvent()
{
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
}

ParticleCollection<GenSimParticle> StrippedEventFile::getGenSimParticles() const
{
    ParticleCollection<GenSimParticle> genParticles;
    for (UInt_t i = 0; i < *genSize; i++) // iterator type must be defined by template to allow comparison with the size variable
    {
        if (genStatus[i] != 1)
        {
            continue;
        }
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(genPt[i],genEta[i], genPhi[i], genMass[i])),
        genPID[i],genStatus[i],genM1[i],genM2[i],genD1[i],genD2[i]);
        //particle.addInfo("Isolation", 0); GAVIN CHANGED
        genParticles.addParticle(particle);
    }
    return genParticles;
}

ParticleCollection<Particle> StrippedEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (UInt_t i = 0; i < *elecSize; i++)
    {
        int charge = elecCharge[i];
        
        Particle::SelectionFit fit;
        if (elecIDPass[i] & 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (elecIDPass[i] & 2) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (elecIDPass[i] & 1) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }

        // Lorentz four-vector
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elecPt[i],elecEta[i], elecPhi[i], elecMass[i])),
        0, 0, charge, ParticleType::electron(), fit);
        particle.addInfo("Isolation", elecReliso[i]);
        recoParticles.addParticle(particle);
    }
    for (UInt_t i = 0; i < *muonSize; i++)
    {
        
        int charge = muonCharge[i];
        
        Particle::SelectionFit fit;
        if (muonIDPass[i] & 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (muonIDPass[i] & 2) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (muonIDPass[i] & 1) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }

        // Lorentz four-vector
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muonPt[i],muonEta[i], muonPhi[i], muonMass[i])),
        0, 0, charge, ParticleType::muon(), fit);
        
        //particle.addInfo("Isolation", muonReliso[i]); GAVIN CHANGED
        recoParticles.addParticle(particle);
        
    }
    return recoParticles;
}

ParticleCollection<Particle> StrippedEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < *jetSize; i++) 
    {
    recoParticles.addParticle(Particle(reco::Candidate::LorentzVector(jetPt[i], jetEta[i], jetPhi[i], jetMass[i]), 0, 0, 0,ParticleType::jet()));      
    }
    return recoParticles;
}

double StrippedEventFile::getMET() const
{
    return static_cast<double>(0);
}

unsigned long long StrippedEventFile::getEventIDNum() const
{
    throw std::runtime_error("StrippedEventFile has no implementation of getEventIDNum");  
}
long StrippedEventFile::getRunNum() const
{
    throw std::runtime_error("StrippedEventFile has no implementation of getRunNum");  
}

bool StrippedEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

std::vector<double> StrippedEventFile::getPDFWeights() const
{
    throw "Function not implemented";
}

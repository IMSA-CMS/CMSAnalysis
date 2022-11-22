#include "CMSAnalysis/DataCollection/interface/StrippedEventFile.hh"


StrippedEventFile::StrippedEventFile(TFile* ifile) : 
    EventFile(ifile),
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
        int charge = -1;
        if (genPID[i] < 0)
        {
            charge = 1;
        }
        if (genPID[i] == 21 || genPID[i] == 22)
        {
            charge = 0;
        }
        genParticles.addParticle(
            Particle(
                reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(genPt[i],genEta[i], genPhi[i], genMass[i])),
                charge, 
                Particle::identifyType(genPID[i]),genPID[i],genStatus[i],genM1[i],genM2[i],genD1[i],genD2[i],
                0)); //not sure if relIso, last parameter, should be set to 0
    }
    return genParticles;
}

ParticleCollection<Particle> StrippedEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (UInt_t i = 0; i < *elecSize; i++)
    {
        
        //std::cout<<"elec_idpass "<<elec_idpass[i]<<std::endl;
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
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(elecPt[i],
                                                                        elecEta[i], elecPhi[i], elecMass[i])),
            charge, ParticleType::electron(), elecReliso[i], fit));
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
        recoParticles.addParticle(Particle(
            reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(muonPt[i],
                                                                        muonEta[i], muonPhi[i], muonMass[i])),
            charge, ParticleType::muon(), muonReliso[i], fit));
        
    }
    // std::cout << recoParticles.getNumParticles() << '\n';
    return recoParticles;
}

ParticleCollection<Particle> StrippedEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < *jetSize; i++) {
        //if(bJet[i] > 0){
            recoParticles.addParticle(
                Particle(reco::Candidate::LorentzVector(jetPt[i], jetEta[i], jetPhi[i], jetMass[i]), 
                0, 
                ParticleType::jet()));    
        //}    
    }
    return recoParticles;
}

double StrippedEventFile::getMET() const
{
    return static_cast<double>(0);
}

bool StrippedEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

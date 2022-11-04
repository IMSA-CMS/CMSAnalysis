#ifndef TREEEVENTFILE_HH
#define TREEEVENTFILE_HH

#include "TFile.h"
#include "EventFile.hh"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>

class TTree;
// Template class allows for variable data type to change depending on file type
// S is for size variables
// E is for elecIdpass
// M is for muonIdpass
template <typename S, typename E, typename M>
class TreeEventFile : public EventFile
{
    public:
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        //virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
    protected:
        struct BranchNames
        {
            std::string elecSize;
            std::string elecCharge;
            std::string elecEta;
            std::string elecPhi;
            std::string elecMass;
            std::string elecPT;
            std::string elecRelIso;

            std::string muonSize;
            std::string muonCharge;
            std::string muonEta;
            std::string muonPhi;
            std::string muonMass;
            std::string muonPT;
            std::string muonRelIso;

            //std::string metSize; useless variable
            std::string metPhi;
            std::string metPT;

            std::string jetSize;
            std::string jetEta;
            std::string jetPhi;
            std::string jetMass;
            std::string jetPT;
            std::string bJet;

            std::string genSize;
            std::string genPid;
            std::string genStatus;
            std::string genEta;
            std::string genPhi;
            std::string genMass;
            std::string genPt;
            std::string gend1;
            std::string gend2;
            std::string genm1;
            std::string genm2;

            std::string elecIdpass;
            std::string muonIdpass;
        };
        TreeEventFile(TFile* ifile, const BranchNames& branchNames);
        virtual std::string getTreeName() = 0;
        //virtual BranchNames getTreeBranches() = 0;
        virtual void initialize();
    protected:

        TTree* tree;
        TTreeReader treeReader;

        mutable TTreeReaderValue<S> elec_size; // nanoAOD wants UInt, Delphes wants Int (for all size variables)
        mutable TTreeReaderArray<Float_t> elec_eta;
        mutable TTreeReaderArray<Float_t> elec_phi;
        mutable TTreeReaderArray<Float_t> elec_mass;
        mutable TTreeReaderArray<Int_t> elec_charge;
        mutable TTreeReaderArray<Float_t> elec_pt;
        mutable TTreeReaderArray<Float_t> elec_reliso;

        mutable TTreeReaderValue<S> muon_size;
        mutable TTreeReaderArray<Float_t> muon_eta;
        mutable TTreeReaderArray<Float_t> muon_phi;
        mutable TTreeReaderArray<Float_t> muon_mass;
        mutable TTreeReaderArray<Int_t> muon_charge;
        mutable TTreeReaderArray<Float_t> muon_pt;
        mutable TTreeReaderArray<Float_t> muon_reliso;

        //mutable TTreeReaderValue<UInt_t> met_size; useless variable
        mutable TTreeReaderArray<Float_t> met_phi;
        mutable TTreeReaderArray<Float_t> met_pt;

        mutable TTreeReaderValue<S> jet_size;
        mutable TTreeReaderArray<Float_t> jet_eta;
        mutable TTreeReaderArray<Float_t> jet_phi;
        mutable TTreeReaderArray<Float_t> jet_mass;
        mutable TTreeReaderArray<Float_t> jet_pt;
        //mutable TTreeReaderArray<Int_t> bJet;

        mutable TTreeReaderValue<S> gen_size;
        mutable TTreeReaderArray<Int_t> gen_pid;
        mutable TTreeReaderArray<Int_t> gen_status;
        mutable TTreeReaderArray<Float_t> gen_eta;
        mutable TTreeReaderArray<Float_t> gen_phi;
        mutable TTreeReaderArray<Float_t> gen_mass;
        mutable TTreeReaderArray<Float_t> gen_pt;
        mutable TTreeReaderArray<Int_t> gen_d1;
        mutable TTreeReaderArray<Int_t> gen_d2;
        mutable TTreeReaderArray<Int_t> gen_m1;
        mutable TTreeReaderArray<Int_t> gen_m2;

        mutable TTreeReaderArray<E> elec_idpass; // nanoAOD wants Int, Delphes wants UInt
        mutable TTreeReaderArray<M> muon_idpass; // nanoAOD wants Bool, Delphes wants UInt
};

template <typename S, typename E, typename M>
inline TreeEventFile<S, E, M>::TreeEventFile(TFile *ifile, const BranchNames& branchNames) : 
    EventFile(ifile), 
    // These objects take the value of the branch. 
    // They change as the treereader advances to the next event. 
    elec_size(treeReader, branchNames.elecSize.c_str()),
    elec_eta(treeReader, branchNames.elecEta.c_str()),
    elec_phi(treeReader, branchNames.elecPhi.c_str()),
    elec_mass(treeReader, branchNames.elecMass.c_str()),
    elec_charge(treeReader, branchNames.elecCharge.c_str()),
    elec_pt(treeReader, branchNames.elecPT.c_str()),
    elec_reliso(treeReader, branchNames.elecRelIso.c_str()),
    muon_size(treeReader, branchNames.muonSize.c_str()),
    muon_eta(treeReader, branchNames.muonEta.c_str()),
    muon_phi(treeReader, branchNames.muonPhi.c_str()),
    muon_mass(treeReader, branchNames.muonMass.c_str()),
    muon_charge(treeReader, branchNames.muonCharge.c_str()),
    muon_pt(treeReader, branchNames.muonPT.c_str()),
    muon_reliso(treeReader, branchNames.muonRelIso.c_str()),
    //met_size(treeReader, branchNames.metSize.c_str()),
    met_phi(treeReader, branchNames.metPhi.c_str()),
    met_pt(treeReader, branchNames.metPT.c_str()),
    jet_size(treeReader, branchNames.jetSize.c_str()),
    jet_eta(treeReader, branchNames.jetEta.c_str()),
    jet_phi(treeReader, branchNames.jetPhi.c_str()),
    jet_mass(treeReader, branchNames.jetMass.c_str()),
    jet_pt(treeReader, branchNames.jetPT.c_str()),
    //bJet(treeReader, branchNames.bJet.c_str()),
    gen_size(treeReader, branchNames.genSize.c_str()),
    gen_pid(treeReader, branchNames.genPid.c_str()),
    gen_status(treeReader, branchNames.genStatus.c_str()),
    gen_eta(treeReader, branchNames.genEta.c_str()),
    gen_phi(treeReader, branchNames.genPhi.c_str()),
    gen_mass(treeReader, branchNames.genMass.c_str()),
    gen_pt(treeReader, branchNames.genPt.c_str()),
    gen_d1(treeReader, branchNames.gend1.c_str()),
    gen_d2(treeReader, branchNames.gend2.c_str()),
    gen_m1(treeReader, branchNames.genm1.c_str()),
    gen_m2(treeReader, branchNames.genm2.c_str()),
    elec_idpass(treeReader, branchNames.elecIdpass.c_str()),
    muon_idpass(treeReader, branchNames.muonIdpass.c_str())
{

}

template <typename S, typename E, typename M>
inline void TreeEventFile<S, E, M>::initialize()
{
    tree = getFile()->Get<TTree>(getTreeName().c_str());
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

template <typename S, typename E, typename M>
inline void TreeEventFile<S, E, M>::nextEvent()
{
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
}

// GenEventInfoProduct TreeEventFile<S, E, M>::getGenInfo() const
// {
//     throw std::runtime_error("Not yet implemented: getGenInfo()");

//     /*
//     edm::Handle<GenEventInfoProduct> genInfo;
//     event->getByLabel(std::string("generator"), genInfo);
//     return *genInfo;
//     */
// }

template <typename S, typename E, typename M>
inline ParticleCollection<Particle> TreeEventFile<S, E, M>::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (S i = 0; i < *elec_size; i++)
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
    for (S i = 0; i < *muon_size; i++)
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
    return recoParticles;
}


template <typename S, typename E, typename M>
inline ParticleCollection<Particle> TreeEventFile<S, E, M>::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(S i = 0; i < *jet_size; i++) {
        //if(bJet[i] > 0){
            recoParticles.addParticle(
                Particle(reco::Candidate::LorentzVector(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]), 
                0, 
                Particle::Type::Jet));    
        //}    
    }
    return recoParticles;
}

template <typename S, typename E, typename M>
inline double TreeEventFile<S, E, M>::getMET() const
{
    return static_cast<double>(met_pt[0]);
}

template <typename S, typename E, typename M>
inline bool TreeEventFile<S, E, M>::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

template <typename S, typename E, typename M>
inline std::vector<bool> TreeEventFile<S, E, M>::getTriggerResults(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

template <typename S, typename E, typename M>
inline std::vector<std::string> TreeEventFile<S, E, M>::getTriggerNames(std::string subProcess) const
{
    throw std::runtime_error("Trigger is not implemented for this file type");
}

#endif
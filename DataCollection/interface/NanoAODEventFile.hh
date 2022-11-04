#ifndef NANOAODEVENTLOADER_HH
#define NANOAODEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class NanoAODEventFile : public TreeEventFile<UInt_t, Int_t, Bool_t> // sets which types to use for nanoAOD files: <size, elecIdpass, muonIdpass>
{
    public:
        NanoAODEventFile(TFile* ifile);
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual void nextEvent() override;
    protected:
        virtual std::string getTreeName() override;
        static BranchNames getTreeBranches();
    private:
        std::vector<GenSimParticle> genSimParticles;
};

#endif
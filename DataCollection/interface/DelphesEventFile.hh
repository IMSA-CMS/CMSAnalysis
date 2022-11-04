#ifndef DELPHESEVENTLOADER_HH
#define DELPHESEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class DelphesEventFile : public TreeEventFile<Int_t, UInt_t, UInt_t> // sets which types to use for Delphes files: <size, elecIdpass, muonIdpass>
{
    public:
        DelphesEventFile(TFile* ifile);
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
    protected:
        virtual std::string getTreeName() override;
        static BranchNames getTreeBranches();
};

#endif
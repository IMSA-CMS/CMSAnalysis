#ifndef NANOAODEVENTLOADER_HH
#define NANOAODEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class NanoAODEventFile : public TreeEventFile<UInt_t, Int_t, Bool_t> // sets which types to use for nanoAOD files: <size, elecIdpass, muonIdpass>
{
    public:
        NanoAODEventFile(TFile* ifile);
    protected:
        virtual std::string getTreeName() override;
        static BranchNames getTreeBranches();
};

#endif
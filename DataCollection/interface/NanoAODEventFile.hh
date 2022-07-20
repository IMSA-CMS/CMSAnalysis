// MORE CHANGES NEEDED
// SO FAR JUST MIRRORS DelphesEventFile
// Dheeran, [07/04/22]

#ifndef NANOAODEVENTLOADER_HH
#define NANOAODEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class NanoAODEventFile : public TreeEventFile
{
    public:
        NanoAODEventFile(TFile* ifile);
    protected:
        virtual std::string getTreeName() override;
        virtual BranchNames getTreeBranches() override;
};

#endif
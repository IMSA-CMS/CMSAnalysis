#ifndef DELPHESEVENTLOADER_HH
#define DELPHESEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class DelphesEventFile : public TreeEventFile
{
    public:
        DelphesEventFile(TFile* ifile);
    protected:
        virtual std::string getTreeName() override;
        virtual BranchNames getTreeBranches() override;
};

#endif
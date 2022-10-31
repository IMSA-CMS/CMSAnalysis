#ifndef STRIPPEDEVENTLOADER_HH
#define STRIPPEDEVENTLOADER_HH

#include "TreeEventFile.hh"

class TTree;

class StrippedEventFile : public TreeEventFile<UInt_t, int, int> // sets which types to use for stripped files: <size, elecIdpass, muonIdpass>
{
    public:
		  StrippedEventFile(TFile* ifile);
    protected:
        virtual std::string getTreeName() override;
        static BranchNames getTreeBranches();
};

#endif
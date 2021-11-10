#ifndef DELPHESEVENTLOADER_HH
#define DELPHESEVENTLOADER_HH

#include "TreeEventLoader.hh"

class TTree;

class DelphesEventLoader : public TreeEventLoader
{
    public:
        using TreeEventLoader::TreeEventLoader;
    protected:
        virtual std::string getTreeName() override;
        virtual BranchNames getTreeBranches() override;
};

#endif
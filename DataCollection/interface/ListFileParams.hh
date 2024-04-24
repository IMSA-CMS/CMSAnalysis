#ifndef LISTFILEPARAMS_HH
#define LISTFILEPARAMS_HH
#include "CMSAnalysis/Utility/interface/FileParams.hh"

class ListFileParams : public FileParams
{
public:
    ListFileParams(std::string filelist);
    virtual std::vector<std::string> getFileList() const override;
private:
    std::string filename;
};
#endif
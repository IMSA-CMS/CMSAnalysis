#ifndef SINGLEFILEPARAMS_HH
#define SINGLEFILEPARAMS_HH
#include "CMSAnalysis/Utility/interface/FileParams.hh"

class SingleFileParams : public FileParams
{
public:
    SingleFileParams(std::string file);
    virtual std::vector<std::string> getFileList() const override;
private:
    std::string filename;
};
#endif
#ifndef PICKFILEPARAMS_HH
#define PICKFILEPARAMS_HH
#include "CMSAnalysis/Utility/interface/FileParams.hh"

class PickFileParams : public FileParams
{
public:
    PickFileParams(std::string process,  std::map<std::string, std::string> parameters);
    virtual std::vector<std::string> getFileList() const override;
private:
    std::string getFileName() const;
};
#endif

#include "CMSAnalysis/DataCollection/interface/SingleFileParams.hh"

SingleFileParams::SingleFileParams(std::string file):
FileParams("", {}),
filename(file)
{

}
std::vector<std::string> SingleFileParams::getFileList() const
{
    return{filename};
}

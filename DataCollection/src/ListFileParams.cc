#include "CMSAnalysis/DataCollection/interface/ListFileParams.hh"
#include <fstream>

ListFileParams::ListFileParams(std::string file):
FileParams("", {}),
filename(file)
{

}
std::vector<std::string> ListFileParams::getFileList() const
{
    std::ifstream textFile(filename);
    std::string line;
    std::vector<std::string> rootFiles;
    while (getline(textFile, line))
    {
        rootFiles.push_back(line);
    }
    return rootFiles;
}
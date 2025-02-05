#include "CMSAnalysis/DataCollection/interface/PickFileParams.hh"
#include <sstream>
#include <fstream>
#include "CMSAnalysis/Utility/interface/Utility.hh"

PickFileParams::PickFileParams(std::string process,  std::map<std::string, std::string> parameters) :
    FileParams(process, parameters)
{}

std::string PickFileParams::getFileName() const
{
    std::ostringstream stream;
    stream << getProcess() << "/" << getProcess();
    for (auto parameter : getParameters())
    {
        stream << "_" << parameter.first << "_" << parameter.second;
    }
    stream << ".txt";
    std::string fileName = stream.str();
    fileName = Utility::substitute(fileName, " ", "_");
    fileName = Utility::getFullPath(fileName);
    return fileName;
}

std::vector<std::string> PickFileParams::getFileList() const
{
    const std::string eossrc = "root://cmsxrootd.fnal.gov//";

    std::string file = getFileName();
    std::vector<std::string> fileList;

    std::ifstream in(file);
    std::string str;
    while (std::getline(in, str))
    {
        if(str.size() > 0)
        {
            fileList.push_back(eossrc + str);
        }
    }
    return fileList;

}

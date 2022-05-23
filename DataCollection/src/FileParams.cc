#include "CMSAnalysis/DataCollection/interface/FileParams.hh"
#include "CMSAnalysis/DataCollection/interface/Utility.hh"
#include <sstream>
#include <fstream>

std::string FileParams::getFileName() const
{
    std::ostringstream stream;
    stream << process;
    for (auto parameter : parameters) 
    {
        stream << "_" << parameter.first << "_" << parameter.second;
    }
    std::string fileName = stream.str();
    return substitute(fileName, " ", "_");
}

std::vector<std::string> FileParams::getFileList() const
{
    std::string file = getFileName();
    std::vector<std::string> fileList;

    std::ifstream in(file);
    std::string str;
    while (std::getline(in, str))
    {
        if(str.size() > 0)
        {
            fileList.push_back(str);
        }
    }   
    return fileList;

}
// call getFileName(), open that file, read from the list into a vector of strings and return
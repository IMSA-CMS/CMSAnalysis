#include "CMSAnalysis/DataCollection/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include <sstream>
#include <fstream>

FileParams::FileParams(std::string iprocess, std::map<std::string, std::string> iparameters):
process(iprocess),
parameters(iparameters)
{

}

std::string FileParams::getFileName() const
{
    std::ostringstream stream;
    stream << "textfiles/" << process << "/" << process;
    for (auto parameter : parameters) 
    {
        stream << "_" << parameter.first << "_" << parameter.second;
    }
    stream << ".txt";
    std::string fileName = stream.str();
    fileName = Utility::substitute(fileName, " ", "_");
    std::cout << "Filename: " << fileName << '\n';
    return fileName;
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
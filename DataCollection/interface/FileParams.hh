#ifndef FILEPARAMS_HH
#define FILEPARAMS_HH
#include <iostream>
#include <map>
#include "IDType.hh"

class FileParams
{
public:
    FileParams(std::string process,  std::map<std::string, std::string> parameters);
    std::string getProcess() const {return process;}
    std::map<std::string, std::string> getParameters() const {return parameters;}
    std::string getFileName() const;
    std::vector<std::string> getFileList() const;
    // call getFileName(), open that file, read from the list into a vector of strings and return

private:
    std::string process;
    std::map<std::string, std::string> parameters;
};
#endif
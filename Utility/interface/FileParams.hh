#ifndef FILEPARAMS_HH
#define FILEPARAMS_HH
#include <iostream>
#include <map>
#include <vector>

class FileParams
{
public:
    FileParams(std::string process,  std::map<std::string, std::string> parameters);
    virtual ~FileParams() {}
    std::string getProcess() const {return process;}
    const std::map<std::string, std::string>& getParameters() const {return parameters;}
    virtual std::vector<std::string> getFileList() const = 0;
    // call getFileName(), open that file, read from the list into a vector of strings and return

private:
    std::string process;
    std::map<std::string, std::string> parameters;
};
#endif
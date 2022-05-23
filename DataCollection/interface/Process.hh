#include <iostream>
#include <vector>
#include <unordered_map>

#include "IDType.hh"
#include "FileParams.hh"

class Process {
public:
    Process(std::string name, std::unordered_map<std::string, std::vector<std::string>> IDType); // std::vector<IDType> IDType
    std::string getName() const{return name;}
    std::vector<IDType> getIDTypes() const {return type;} 
    FileParams getParams(std::vector<std::string>, double crossSection) const;

private:
    std::string name;
    std::vector<IDType> type;
};
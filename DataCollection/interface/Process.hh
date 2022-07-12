#include <iostream>
#include <vector>
#include <unordered_map>

#include "IDType.hh"
#include "FileParams.hh"

class Process {
public:
    Process(std::string name, std::vector<IDType> type); // std::vector<IDType> IDType
    Process(std::string name, std::unordered_map<std::string, std::vector<std::string>> type);
    std::string getName() const{return name;}
    std::vector<IDType> getIDTypes() const {return type;} 

    // from the params vector (which is ordered category, value for some reason), it checks if
    // these categories and values are in the type vector, inserts them into a map and then a FileParams object, 
    // which it returns. 
    FileParams getParams(std::vector<std::string> params, double crossSection) const;
private:
    std::string name;
    std::vector<IDType> type;
};
#ifndef PROCESSDICTIONARY_HH
#define PROCESSDICTIONARY_HH
#include <vector> 
#include <memory>
#include "Process.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"

class ProcessDictionary
{
    public: 
    // Reads information from pickfiles and adds a process to the vector processes using it
    void loadProcesses(std::string filename);
    
    // Parses through the textfiles to find the parameters needed, creates a vector of FileParams
    std::vector<std::shared_ptr<FileParams>> readFile(std::string filename);

    //Checks if the process name is found in processes.txt
    Process& findProcess(std::string newProcess);

    void checkIDTypes(std::vector<IDType> idtypes, std::unordered_map<std::string, std::vector<std::string>> typesToCheck);

    std::shared_ptr<FileParams> makeFileparams(Process& processName, std::vector<std::pair<std::string, std::string>> pairs);

    // makes the vector of pairs that are passed into makeFileParams
    std::shared_ptr<FileParams> getFileparams(Process& processName, std::unordered_map<std::string, std::vector<std::string>> types, std::vector<int> indices);
    private:
    std::vector<Process> processes;
    std::unordered_map<std::string, double> weights;
};
//add all functions to the header file.
// move weightingmodule to constructor.


#endif
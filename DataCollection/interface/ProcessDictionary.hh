#ifndef PROCESSDICTIONARY_HH
#define PROCESSDICTIONARY_HH
#include <vector> 
#include "Process.hh"

class ProcessDictionary
{
    public: 
    // Reads information from pickfiles and adds a process to the vector processes using it
    void loadProcesses(std::string filename);
    
    // Parses through the textfiles to find the parameters needed, creates a vector of FileParams
    std::vector<FileParams> readFile(std::string filename);

    //Checks if the process name is found in processes.txt
    Process& findProcess(std::string newProcess);

    void checkIDTypes(std::vector<IDType> idtypes, std::unordered_map<std::string, std::vector<std::string>> typesToCheck);

    FileParams makeFileparams(Process& processName, std::vector<std::pair<std::string, std::string>> pairs);

    // makes the vector of pairs that are passed into makeFileParams
    FileParams getFileparams(Process& processName, std::unordered_map<std::string, std::vector<std::string>> types, std::vector<int> indices);
    private:
    std::vector<Process> processes;
    std::unordered_map<std::string, double> weights;
};
//add all functions to the header file.
// move weightingmodule to constructor.


#endif
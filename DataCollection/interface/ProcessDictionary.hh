#ifndef PROCESSDICTIONARY_HH
#define PROCESSDICTIONARY_HH
#include <vector> 
#include "Process.hh"

class ProcessDictionary
{
    public: 
    void loadProcesses(std::string filename);
    std::vector<FileParams> readFile(std::string filename);
    Process& findProcess(std::string processName);
    FileParams makeFileparams(Process& processName, std::vector<std::pair<std::string, std::string>> pairs);
    FileParams getFileparams(Process& processName, std::unordered_map<std::string, std::vector<std::string>> types, std::vector<int> indices);
    private:
    std::vector<Process> processes;
    std::unordered_map<std::string, double> weights;
};
//add all functions to the header file.
// move weightingmodule to constructor.


#endif
#ifndef PROCESS_HH
#define PROCESS_HH

#include "SingleProcess.hh"

#include <string>
#include <vector>

class TH1;

class Process
{
    public:
        Process(std::string name, int color) : name(name), color(color) {}
        std::string getName() const {return name;}
        int getColor() const {return color;}
        std::vector<SingleProcess> getProcesses() const {return processes;}
        TH1* getHist(bool scaleToExpected = false) const;
        void addProcess(SingleProcess process) {processes.push_back(process);}

    private:
        const std::string name;
        const int color;
        std::vector<SingleProcess> processes;
};

#endif
#ifndef PROCESS_HH
#define PROCESS_HH

#include "SingleProcess.hh"
#include "HistVariable.hh"
#include "TH2F.h"
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
        //Gets the hists from all the singleProcesses as one hist
        TH1* getHist(HistVariable histType, bool scaleToExpected = false) const;
        TH2* get2DHist(HistVariable histType) const;
        void addProcess(SingleProcess process);
        //Gets the total yield of all singleProcesses
        double getYield(HistVariable dataType) const;
        //Returns table format data of all singleProcesses
        //std::vector<std::vector<std::string>> getData() const;

    private:
        const std::string name;
        const int color;
        std::vector<SingleProcess> processes;
};

#endif
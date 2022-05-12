#ifndef PROCESS_HH
#define PROCESS_HH

#include "SingleProcess.hh"

#include <string>
#include <vector>

class TH1;

class Process
{
    public:
        Process(std::string name, int color, double eventsExpected, bool signalSwitch = 1) : name(name), color(color), eventsExpected(eventsExpected), signalSwitch(signalSwitch) {}
        std::string getName() const {return name;}
        double getEventsExpected() const {return eventsExpected;}
        bool getSignalSwitch() const {return signalSwitch;}
        int getColor() const {return color;}
        TH1* getHist() const;
        void addProcess(SingleProcess process) {processes.push_back(process);}

    private:
        const std::string name;
        const int color;
        const double eventsExpected;
        const bool signalSwitch;
        std::vector<SingleProcess> processes;
};

#endif
#ifndef PROCESS_HH
#define PROCESS_HH

#include "SingleProcess.hh"

#include <string>
#include <vector>

class TH1;

class Process
{
    public:
        std::string getName() const {return name;}
        double getEventsExpected() const {return eventsExpected;}
        bool getSignalSwitch() const {return signalSwitch;}
        int getColor() const {return color;}
        TH1* getHist() const;

    private:
        const std::string name;
        const double eventsExpected;
        const bool signalSwitch;
        const int color; 
        std::vector<SingleProcess> processes;
};

#endif
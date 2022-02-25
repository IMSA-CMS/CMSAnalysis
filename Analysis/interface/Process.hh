#ifndef PROCESS_HH
#define PROCESS_HH

#include <string>
#include <vector>


class Process
{
public:
    std::string getName() const {return name;}
    double eventsExpected() const {return eventsExpected;}
    bool signalSwitch() const {return signalSwitch;}
    int getColor() const {return color;}
    
    

private:
    const std::string name;
    const double eventsExpected;
    const bool signalSwitch;
    const int color; 
    std::vector<SingleProcess> processes;

};

#endif
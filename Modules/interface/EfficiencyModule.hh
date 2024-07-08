#ifndef EFFICIENCYMODULE_HH
#define EFFICIENCYMODULE_HH
#include "AnalysisModule.hh"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

//Analysis module to calculate efficiencies
//This is a class which other efficiency classes derrive from
//This module also has the ability to calculate efficiencies itself 
class EfficiencyModule : public AnalysisModule
{
    public:
        void finalize() override;
        virtual std::string getName() override {return "EfficiencyModule";}

    protected:
        //Adds the counter for a thing
        void incrementCounter(std::string name, double increment = 1);

        //Returns the value connected to a specific counter
        double getCounter(std::string name) const;

        //Returns the a counter value over the sum of all counters
        double getEfficiency(std::string name) const;

        bool process() override;

        //Implemented in derrived classes to update counters in a specified way
        virtual void doCounters() = 0; 

    private:
        std::unordered_map<std::string, double> counters;
        double total;
};

#endif

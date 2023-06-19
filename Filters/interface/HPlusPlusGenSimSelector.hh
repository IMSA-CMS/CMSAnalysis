#ifndef HPLUSPLUSGENSIMSELECTOR_HH
#define HPLUSPLUSGENSIMSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

//Selects relevant particles for GenSim H++ Events (and feeds them to an EventModule)
//Implement in a DataCollectionPlan with a shared pointer then add to an EventModule with ->addSelector() function

class InputModule;

class HPlusPlusGenSimSelector : public Selector
{
    public:
        //Selects H++s, their Lepton Daughters, and Z Bosons
        void selectParticles(const InputModule* input, Event& event) const override;
};

#endif
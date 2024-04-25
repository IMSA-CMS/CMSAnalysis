#ifndef MLSTRIPCALCULATOR_HH
#define MLSTRIPCALCULATOR_HH

#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "TMVA/Reader.h"

class SpecialVariableModule;


class MLCalculator : public ProductionModule
{

    
    public:
    MLCalculator(std::shared_ptr<SpecialVariableModule> specialVariable, std::string fileName, std::string method); //do the same thing for recomod in leptonjetMLStripModule
    virtual void initialize() override;
    virtual bool process() override {return true;}
    double getOutput(ParticleCollection<Particle> part) const;





    private:
    std::shared_ptr<SpecialVariableModule> specialVariable;//same here
    mutable TMVA::Reader reader;
    std::string fileName;
    std::string method;

};





#endif
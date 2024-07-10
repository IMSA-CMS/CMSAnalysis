#ifndef HIGGSCOMBINESTRIPMODULE_HH
#define HIGGSCOMBINESTRIPMODULE_HH

#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"

class HiggsCombineStripModule: public SpecialVariableModule
{
    
    public:
    HiggsCombineStripModule(); //do the same thing for recomod in leptonjetMLStripModule

    virtual std::string getName() override {return "HiggsCombineStripModule";}
   
    private:


    protected:
    virtual void addVariables() override;

	virtual void calculateVariables(ParticleCollection<Particle> particle) override;

};
#endif
#ifndef LEPTONJETMLSTRIPMODULE_HH
#define LEPTONJETMLSTRIPMODULE_HH

#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"

class LeptonJetReconstructionModule;


class LeptonJetMLStripModule: public SpecialVariableModule
{
    
    public:
    LeptonJetMLStripModule(); //do the same thing for recomod in leptonjetMLStripModule

    virtual std::string getName() override {return "LeptonJetMLStripModule";}


    protected:
    virtual void addVariables() override;

	virtual void calculateVariables(ParticleCollection<Particle> particle) override;


};








#endif
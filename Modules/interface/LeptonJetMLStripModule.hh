#ifndef LEPTONJETMLSTRIPMODULE_HH
#define LEPTONJETMLSTRIPMODULE_HH

#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"

class LeptonJetReconstructionModule;


class LeptonJetMLStripModule: public SpecialVariableModule
{
    
    public:
    LeptonJetMLStripModule(); //do the same thing for recomod in leptonjetMLStripModule

    private:


    protected:
    virtual void addVariables() override;

	virtual void calculateVariables(Particle particle) override;


};








#endif
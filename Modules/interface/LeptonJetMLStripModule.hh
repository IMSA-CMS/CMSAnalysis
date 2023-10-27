#ifndef LEPTONJETMLSTRIPMODULE_HH
#define LEPTONJETMLSTRIPMODULE_HH

#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"

class LeptonJetReconstructionModule;


class LeptonJetMLStripModule: public SpecialVariableModule
{
    
    public:
    LeptonJetMLStripModule(std::shared_ptr<LeptonJetReconstructionModule> recomod); //do the same thing for recomod in leptonjetMLStripModule

    private:
    std::shared_ptr<LeptonJetReconstructionModule> recomod;//same here


    protected:
    virtual void addVariables() override;

	virtual void calculateVariables() override;
};








#endif
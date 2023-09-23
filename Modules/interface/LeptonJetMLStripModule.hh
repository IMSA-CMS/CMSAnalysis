#ifndef LEPTONJETMLSTRIPMODULE_HH
#define LEPTONJETMLSTRIPMODULE_HH

#include "TreeMakerModule.hh"

class LeptonJetMLStripModule:
    public TreeMakerModule
{
    virtual void addVariables() override;

	virtual void calculateVariables() override;
};








#endif
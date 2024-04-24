#ifndef TREEMAKERMODULE_HH
#define TREEMAKERMODULE_HH

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"


class SpecialVariableModule;
class TTree;

class TreeMakerModule : public AnalysisModule
{

    
    public:
    TreeMakerModule(std::shared_ptr<SpecialVariableModule> specialVariable, std::string name); //do the same thing for recomod in leptonjetMLStripModule
    virtual bool process() override;
    virtual void initialize() override;
    virtual void finalize() override;






    private:
    std::shared_ptr<SpecialVariableModule> specialVariable;//same here
    TTree* tree;
    std::string name;

};





#endif
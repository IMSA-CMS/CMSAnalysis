#ifndef TREEMAKERMODULE_HH
#define TREEMAKERMODULE_HH

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

#include "TTree.h"

class SpecialVariableModule;

class TreeMakerModule : public AnalysisModule
{
    public:
        TreeMakerModule(std::shared_ptr<SpecialVariableModule> specialVariable, std::string name); //do the same thing for recomod in leptonjetMLStripModule
        virtual bool process() override;
        virtual void initialize() override;
        virtual void finalize() override;

        virtual std::string getName() override {return "TreeMakerModule";}

    protected:
        virtual void fillTree() {tree->Fill();}
        virtual std::vector<ParticleCollection<Particle>> getParticles() const = 0;
        std::shared_ptr<SpecialVariableModule> getSpecialVariable() {return specialVariable;}
        std::string getTreeName() {return name;}

    private:
    std::shared_ptr<SpecialVariableModule> specialVariable;//same here
    TTree* tree;
    std::string name;

};





#endif
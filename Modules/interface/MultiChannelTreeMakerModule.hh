#ifndef MULTICHANNELTREEMAKERMODULE_HH
#define MULTICHANNELTREEMAKERMODULE_HH

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"

#include "TTree.h"

class SpecialVariableModule;

class MultiChannelTreeMakerModule : public TreeMakerModule{
    public:
        MultiChannelTreeMakerModule(std::shared_ptr<SpecialVariableModule> specialVariable, std::string name);
        virtual bool process() override;
        virtual void initialize() override;
        virtual void finalize() override;

        virtual std::string getName() override {return "MultiChannelTreeMakerModule";}

    protected:
        void fillTree() override;
        virtual std::vector<ParticleCollection<Particle>> getParticles() const override;
    private:
        std::map<std::string, TTree*> trees;
};


#endif
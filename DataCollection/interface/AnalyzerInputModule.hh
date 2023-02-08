#ifndef ANALYZERINPUTMODULE_HH
#define ANALYZERLOADERINPUTMODULE_HH

#include <memory>

#include "Module.hh"
#include "ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
class TFile;
class EventLoader;
class EventInterface;

class AnalyzerInputModule : public InputModule
{
    public:
        AnalyzerInputModule(const EventInterface** eventInterface);

        virtual ParticleCollection<Lepton> getLeptons(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getParticles(RecoLevel level, const ParticleType& particleType = ParticleType::none()) const override;
        virtual ParticleCollection<Particle> getJets(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getSpecial(std::string key) const override {throw std::runtime_error("getSpecial not implemented in EventLoaderInputModule");}
        virtual int getNumPileUpInteractions() const override;

        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual double getMET() const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override;
    private:
        const EventInterface** eventInterface;
};
#endif
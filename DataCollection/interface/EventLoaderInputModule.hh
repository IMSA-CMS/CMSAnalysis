#ifndef EVENTLOADERINPUTMODULE_HH
#define EVENTLOADERINPUTMODULE_HH

#include <memory>

#include "Module.hh"
#include "ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class EventLoaderInputModule : public EventLoaderInputModule
{
    public:
        EventLoaderInputModule(const EventLoader *iEventLoader);
        virtual void setLeptonSelector(std::shared_ptr<Selector> selector) {leptonSelector = selector;}

        virtual ParticleCollection getLeptons(RecoLevel level) const override;
        // not sure this works
        virtual ParticleCollection getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None,
        std::shared_ptr<Selector> selector = nullptr) const override;
        virtual ParticleCollection getJets(RecoLevel level, double pTCut = 0) const override;

        virtual GenEventInfoProduct getGenInfo() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual double getMET() const override;
    private:
        const EventLoader *eventLoader;
        std::shared_ptr<Selector> leptonSelector;
}
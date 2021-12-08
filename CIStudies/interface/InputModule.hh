#ifndef INPUTMODULE_HH
#define INPUTMODULE_HH

#include <memory>

#include "Module.hh"
#include "ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

class TFile;
class EventLoader;
class Selector;

class InputModule
{
public:
    InputModule(const EventLoader *iEventLoader);

    enum class RecoLevel
    {
        Reco,
        GenSim
    };

    void setLeptonSelector(std::shared_ptr<Selector> selector) {leptonSelector = selector;}

    ParticleCollection getLeptons(RecoLevel level, std::shared_ptr<Selector> selector = nullptr) const;
    // not sure this works
    ParticleCollection getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None,
    std::shared_ptr<Selector> selector = nullptr) const;

    GenEventInfoProduct getGenInfo() const;
    // edm::TriggerResults getTriggerResults(std::string subProcess) const;
    // edm::TriggerNames getTriggerNames(std::string subProcess) const;
    double getMET() const;

private:
    const EventLoader *eventLoader;
    std::shared_ptr<Selector> leptonSelector;
};

#endif

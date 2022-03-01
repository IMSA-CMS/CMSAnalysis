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
    virtual ~InputModule(){}

    enum class RecoLevel
    {
        Reco,
        GenSim
    };

    virtual void setLeptonSelector(std::shared_ptr<Selector> selector) {leptonSelector = selector;}

    virtual ParticleCollection getLeptons(RecoLevel level) const = 0;
    // not sure this works
    virtual ParticleCollection getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None,
    std::shared_ptr<Selector> selector = nullptr) const = 0;
    virtual ParticleCollection getJets(RecoLevel level, double pTCut = 0) const = 0;

    virtual GenEventInfoProduct getGenInfo() const = 0;
    virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
    virtual std::vector<std::string> getTriggerNames(std::string subProcess) const=0;
    virtual double getMET() const = 0;

private:
    std::shared_ptr<Selector> leptonSelector;
};

#endif

#ifndef INPUTMODULE_HH
#define INPUTMODULE_HH

#include <memory>

#include "Module.hh"
#include "ParticleCollection.hh"
#include "Lepton.hh"
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

    virtual ParticleCollection<Lepton> getLeptons(RecoLevel level) const = 0;
    virtual ParticleCollection<Particle> getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None) const = 0;
    virtual ParticleCollection<Particle> getJets(RecoLevel level) const = 0;
    virtual Particle getSpecial(std::string name) const = 0;

    //virtual GenEventInfoProduct getGenInfo() const = 0;
    virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
    virtual std::vector<std::string> getTriggerNames(std::string subProcess) const=0;
    virtual double getMET() const = 0;

    virtual bool checkTrigger(std::string triggerName, std::string subProcess = "") const = 0;
private:
};

#endif

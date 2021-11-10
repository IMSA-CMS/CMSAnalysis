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

class InputModule
{
    public:
        InputModule(const EventLoader* iEventLoader);
        // void setFile(TFile* tfile) {file = tfile;}
        enum class RecoLevel
        {
            Reco, GenSim
        };
        ParticleCollection getLeptons(RecoLevel level) const;
        // not sure this works
        ParticleCollection getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None) const;
        //std::vector<PileupSummaryInfo> getPileupInfo() const;
        GenEventInfoProduct getGenInfo() const;
        // edm::TriggerResults getTriggerResults(std::string subProcess) const;
        // edm::TriggerNames getTriggerNames(std::string subProcess) const;
        double getMET() const;
    protected:
        // const edm::EventBase& getEvent();
        // TFile* getFile(); //{return file;}
    private:
        const EventLoader* eventLoader;
};



#endif
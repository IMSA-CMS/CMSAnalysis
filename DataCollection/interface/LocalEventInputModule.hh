#ifndef LOCALEVENTINPUTMODULE_HH
#define LOCALEVENTINPUTMODULE_HH

#include <memory>

#include "CMSAnalysis/DataCollection/interface/Module.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
class TFile;


class LocalEventInputModule : public InputModule
{
    public:
        LocalEventInputModule(const Event* event1);

        virtual ParticleCollection<Lepton> getLeptons(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getParticles(RecoLevel level, Particle::Type particleType = Particle::Type::None) const override;
        virtual ParticleCollection<Particle> getJets(RecoLevel level) const override;
        //virtual ParticleCollection<Particle> getSpecials(Recolevel level) const;

        //virtual GenEventInfoProduct getGenInfo() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual double getMET() const override;
    private:
        const Event* event = nullptr;
};
#endif
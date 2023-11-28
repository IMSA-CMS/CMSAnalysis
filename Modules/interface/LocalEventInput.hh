#ifndef LOCALEVENTINPUT_HH
#define LOCALEVENTINPUT_HH

#include <memory>

#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/RootEventInterface.hh"
class TFile;


class LocalEventInput : public EventInput
{
    public:
        LocalEventInput(const Event* event1);

        virtual ParticleCollection<Lepton> getLeptons(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getParticles(RecoLevel level = EventInput::RecoLevel::Reco, const ParticleType& particleType = ParticleType::none()) const override;
        virtual ParticleCollection<Particle> getJets(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getSpecial(std::string key) const override;
        virtual int getNumPileUpInteractions() const override;

        virtual std::vector<double> getPDFWeights() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual double getMET() const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override;
    private:
        const Event* event = nullptr;
        const RootEventInterface* eInterface = nullptr;
};
#endif
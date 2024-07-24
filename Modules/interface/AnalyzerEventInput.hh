#ifndef ANALYZEREVENTINPUT_HH
#define ANALYZEREVENTINPUT_HH

#include <memory>

#include "Module.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
class TFile;
class EventLoader;
class EventInterface;

class AnalyzerEventInput : public EventInput
{
    public:
        AnalyzerEventInput(const EventInterface** eventInterface);

        virtual ParticleCollection<Lepton> getLeptons(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getParticles(RecoLevel level, const ParticleType& particleType = ParticleType::none()) const override;
        virtual ParticleCollection<Particle> getJets(RecoLevel level) const override;
        virtual ParticleCollection<Particle> getSpecial(std::string key) const override {throw std::runtime_error("getSpecial not implemented in EventLoaderEventInput");}
        virtual int getNumPileUpInteractions() const override;

        virtual std::vector<double> getPDFWeights() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual double getMET() const override;
        virtual unsigned long long getEventIDNum() const override; 
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override;

        virtual const std::shared_ptr<FileParams> getFileParams() const override;
        
    private:
        const EventInterface** eventInterface;
};
#endif
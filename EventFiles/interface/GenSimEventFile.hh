#ifndef GENSIMEVENTFILE_HH
#define GENSIMEVENTFILE_HH

#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class TFile;

//GenSim implementation of root file
//Created in EventLoader
//Member functions are accessed in EventInputs
class GenSimEventFile : public EventFile
{
    public:
        GenSimEventFile(TFile* ifile, std::shared_ptr<FileParams> iparams);
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override; //not implemented
        virtual ParticleCollection<Particle> getRecoJets() const override; //not implemented
        virtual int getLumiBlock() const override {throw std::runtime_error("GenSim files do not contain luminosity information");}
        virtual int getNumOfEvents() const override {return event->size();}
        virtual int getNumPileUpInteractions() const override; //not implemented
        virtual reco::Candidate::LorentzVector getMET() const override;
        virtual std::vector<double> getPDFWeights() const override;
        virtual unsigned long long getEventIDNum() const override; 
        virtual long getRunNum() const override; 
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override; 
    private:
        std::shared_ptr<fwlite::Event> event = nullptr;
};



#endif

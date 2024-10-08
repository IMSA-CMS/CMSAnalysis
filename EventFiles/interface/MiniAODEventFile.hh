#ifndef MINIAODEVENTFILE_HH
#define MINIAODEVENTFILE_HH

#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class TFile;

class MiniAODEventFile : public EventFile
{
    public:
        MiniAODEventFile(TFile* ifile, std::shared_ptr<FileParams> iparams);
        // maybe just ++event
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual int getNumPileUpInteractions() const override;
        virtual int getNumOfEvents() const override {return event->size();}
        virtual std::vector<double> getPDFWeights() const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override; //checks if event passes trigger criteria
    protected:
        //virtual void getLeptons(RecoLevel level) override;
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const override;
        virtual double getMET() const override;
        virtual unsigned long long getEventIDNum() const override; 
        virtual long getRunNum() const override; 
    private:
        std::shared_ptr<fwlite::Event> event = nullptr;
};



#endif

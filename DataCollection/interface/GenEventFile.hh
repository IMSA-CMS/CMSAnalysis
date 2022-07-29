#ifndef GENEVENTFILE_HH
#define GENEVENTFILE_HH

#include "EventLoader.hh"
#include "ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class TFile;

class GenEventFile : public EventFile
{
    public:
        GenEventFile(TFile* ifile);
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        //virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        
    private:
        std::shared_ptr<fwlite::Event> event = nullptr;
};



#endif

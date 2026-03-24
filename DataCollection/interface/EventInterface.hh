#ifndef EVENTINTERFACE_HH
#define EVENTINTERFACE_HH

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

class FileParams;

class EventInterface
{
    public:
        virtual ~EventInterface();
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const = 0;
        
        virtual ParticleCollection<Particle> getRecoParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoJets() const = 0;
        virtual reco::Candidate::LorentzVector getMET() const = 0;
        virtual std::vector<double> getPDFWeights() const = 0;
        virtual unsigned long long getEventIDNum() const = 0; 
        virtual long getRunNum() const = 0;
        virtual int getLumiBlock() const = 0;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const = 0;
        virtual int getNumPileUpInteractions() const = 0;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const = 0;
        virtual const std::shared_ptr<FileParams> getFileParams() const = 0;
};

#endif
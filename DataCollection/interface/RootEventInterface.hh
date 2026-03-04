#ifndef ROOTEVENTINTERFACE_HH
#define ROOTEVENTINTERFACE_HH
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "EventInterface.hh"
#include "CMSAnalysis/EventFiles/interface/EventFile.hh"
class EventFile;

class RootEventInterface : public EventInterface
{
    public: 
        RootEventInterface(){}
        RootEventInterface(std::shared_ptr<EventFile> ifile): eventFile(ifile) {}
        void setFile(std::shared_ptr<EventFile> eFile){eventFile = eFile;}
        //Call event file functions
        ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        ParticleCollection<Particle> getRecoParticles() const override;
        ParticleCollection<Particle> getRecoJets() const override;
        reco::Candidate::LorentzVector getMET() const override;
        std::vector<double> getPDFWeights() const override;
        unsigned long long getEventIDNum() const override; 
        long getRunNum() const override; 
        virtual int getLumiBlock() const override;
        std::vector<bool> getTriggerResults(std::string subProcess) const override;
        std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual int getNumPileUpInteractions() const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override;
        virtual const std::shared_ptr<FileParams> getFileParams() const override; 
        
    private:   
        std::shared_ptr<EventFile> eventFile;
};

#endif
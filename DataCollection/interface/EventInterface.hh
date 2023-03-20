#ifndef EVENTINTERFACE_HH
#define EVENTINTERFACE_HH

#include "ParticleCollection.hh"

class EventInterface
{
    public:
        virtual ~EventInterface();
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const = 0;
        
        virtual ParticleCollection<Particle> getRecoParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoJets() const = 0;
        virtual double getMET() const = 0;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const = 0;
        virtual int getNumPileUpInteractions() const = 0;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const = 0;

};

#endif
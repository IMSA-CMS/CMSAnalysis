#ifndef LEPTONJETDUMPMODULE_HH
#define LEPTONJETDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "ParticleCollection.hh"
#include "GenSimParticle.hh"
#include "Particle.hh"
#include "LeptonJetMatchingModule.hh"
#include "MatchingModule.hh"

class LeptonJetDumpModule : public AnalysisModule
{
    public:
    LeptonJetDumpModule(std::shared_ptr<LeptonJetMatchingModule> iLepJet,std::shared_ptr<MatchingModule> iMatch, int inumEvents = -1);

    virtual bool process() override;
  
    virtual void writeAll() override;
    
    void printLeptons(std::ofstream& my_file);
    void printRecoJets(std::ofstream& my_file);
    private:

    int counter;
    std::shared_ptr<LeptonJetMatchingModule> lepJet;
    std::shared_ptr<MatchingModule> matchMod;
    int numEvents;
};

#endif
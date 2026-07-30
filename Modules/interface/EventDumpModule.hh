#ifndef EVENTDUMPMODULE_HH
#define EVENTDUMPMODULE_HH

#include "AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <vector>

// Event Dump Module is responsible for printing the particle information such as energy, pt, phi, eta for all the
// events in the root file There is a flag for genSim or reco in constructor GenSim events print daughter and mother
// information along with status codes used by PYTHIA EventDumpModule should be added into any Analyzerplan by making a
// shared pointer, and adding onto analysis module
class EventDumpModule : public AnalysisModule
{
  public:
    EventDumpModule(bool iGenSim, bool iReco, int inumOfEvents = -1, std::shared_ptr<MatchingModule> mod = nullptr);

    bool process() override;

    void finalize() override;

    void printGenSimParticleCollection(const ParticleCollection<GenSimParticle> &genParts,
                                       std::ofstream &my_file) const;
    void printRecoParticleCollection(const ParticleCollection<Particle> &recoParts, std::ofstream &my_file) const;
    static std::string formatDaughterParticles(const GenSimParticle &part, const std::vector<GenSimParticle> &genParts);

    std::string getName() override
    {
        return "EventDumpModule";
    }
    static std::string formatMotherParticle(const GenSimParticle &part, const std::vector<GenSimParticle> &genParts);

  private:
    // prints information to the screen
    // void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file)
    // const;

    int getLatestIndexOfDaughters(const GenSimParticle &part, const std::vector<GenSimParticle> &genParts) const;

    bool genSim;
    bool reco;
    int numOfEvents;
    int counter;
    int chargeMisIdCounter = 0;
    int photonFakeCounter = 0;
    int unmatchedLepCounter = 0;

    std::shared_ptr<MatchingModule> matchingMod;
};

std::ostream &operator<<(std::ostream &str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles);
std::ostream &operator<<(std::ostream &str, const GenSimParticle part);

#endif

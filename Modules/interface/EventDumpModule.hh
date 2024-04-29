#ifndef EVENTDUMPMODULE_HH
#define EVENTDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

// Event Dump Module is responsible for printing the particle information such as energy, pt, phi, eta for all the events in the root file
// There is a flag for genSim or reco in constructor
// GenSim events print daughter and mother information along with status codes used by PYTHIA 
// EventDumpModule should be added into any Analyzerplan by making a shared pointer, and adding onto analysis module
class EventDumpModule : public AnalysisModule
{
public:
  EventDumpModule(bool iGenSim, bool iReco, int inumOfEvents = -1);

  virtual bool process() override;
  
  void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;
  void printRecoParticleCollection(const ParticleCollection<Particle>& recoParts, std::ofstream& my_file) const;
  static std::string formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts);

private:
  // prints information to the screen
  //void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;
  
  static int getIndexOf(const Particle& part, const std::vector<Particle>& allParts);
  static int getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts);
  int getLatestIndexOfDaughters(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;
  static std::string formatMotherParticle(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts);

  bool genSim;
  bool reco;
  bool clearLatch;
  int numOfEvents;
  int counter;
};

std::ostream& operator<<(std::ostream& str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles);
std::ostream& operator<<(std::ostream& str, const GenSimParticle part);

#endif


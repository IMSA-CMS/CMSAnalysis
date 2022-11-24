#ifndef EVENTDUMPMODULE_HH
#define EVENTDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "ParticleCollection.hh"
#include "GenSimParticle.hh"
#include "Particle.hh"

// GenSimIdentificationModule creates a Particle Collection
// of gen sim particles that will be used in MatchingModule 
class EventDumpModule : public AnalysisModule
{
public:
  EventDumpModule(bool iGenSim, bool iReco, int inumOfEvents = -1);

  virtual bool process() override;
  
  virtual void writeAll() override;
  void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;
  void printRecoParticleCollection(const ParticleCollection<Particle>& recoParts, std::ofstream& my_file) const;
  static std::string formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts);

private:
  // prints information to the screen
  //void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;

  static int getIndexOf(const Particle& part, const std::vector<Particle>& allParts);
  static int getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts);
  int getLatestIndexOfDaughters(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;

  bool genSim;
  bool reco;
  bool clearLatch;
  int numOfEvents;
  int counter;
};


#endif



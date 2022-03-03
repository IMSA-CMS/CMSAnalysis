#ifndef GENSIMEVENTDUMPMODULE_HH
#define GENSIMEVENTDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "ParticleCollection.hh"

namespace edm
{
  class EventBase;
}

namespace reco
{
  class GenParticle;
}

// GenSimIdentificationModule creates a Particle Collection
// of gen sim particles that will be used in MatchingModule 
class GenSimEventDumpModule : public AnalysisModule
{
public:
  GenSimEventDumpModule(int inumOfEvents = -1);

  virtual bool process() override;
  
  virtual void writeAll() override;

private:
  // prints information to the screen
  void printGenParticleCollection(const ParticleCollection& genParts) const;

  int getIndexOf(const Particle& part, const std::vector<Particle>& genParts) const;
  int getLatestIndexOfDaughters(const Particle& part, const std::vector<Particle>& genParts) const;
  std::string formatDaughterParticles(const Particle& part, const std::vector<Particle>& genParts) const;

  int numOfEvents;
  int counter;
};


#endif



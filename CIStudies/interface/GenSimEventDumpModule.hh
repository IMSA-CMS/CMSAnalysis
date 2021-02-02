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
  virtual bool process(const edm::EventBase& event) override;
  
  virtual void writeAll() override;

private:
  // prints information to the screen
  void printGenParticleCollection(const std::vector<reco::GenParticle>& genParts) const;

  int getIndexOf(const reco::Candidate* part, const std::vector<reco::GenParticle>& genParts) const;

};

#endif



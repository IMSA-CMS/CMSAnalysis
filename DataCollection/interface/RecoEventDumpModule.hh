#ifndef RECOEVENTDUMPMODULE_HH
#define RECOEVENTDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "ParticleCollection.hh"
#include "Particle.hh"

// Outputs relevant data for reco events to a text file.
// Not very finished/polished, code is mostly copied from GenSimEventDumpModule
class RecoEventDumpModule : public AnalysisModule
{
public:
  RecoEventDumpModule(int inumOfEvents = -1);

  virtual bool process() override;
  
  virtual void writeAll() override;

private:
  // prints information to the screen
  void printRecoParticleCollection(const ParticleCollection<Particle>& recoParts, std::ofstream& my_file) const;

  int getIndexOf(const Particle& part, const std::vector<Particle>& recoParts) const;

  int numOfEvents;
  int counter;
  int selectedcounter = 0;
};


#endif



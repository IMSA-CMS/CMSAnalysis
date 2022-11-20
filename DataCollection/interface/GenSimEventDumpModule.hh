#ifndef GENSIMEVENTDUMPMODULE_HH
#define GENSIMEVENTDUMPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "ParticleCollection.hh"
#include "GenSimParticle.hh"

// 
// of gen sim particles that will be used in MatchingModule 
class GenSimEventDumpModule : public AnalysisModule
{
public:
  GenSimEventDumpModule(int inumOfEvents = -1);

  virtual bool process() override;
  
  virtual void writeAll() override;

private:
  // prints information to the screen
  void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;

  int getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;
  int getLatestIndexOfDaughters(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;
  std::string formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;
    std::string formatMotherParticle(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;

  int numOfEvents;
  int counter;

  bool clearlatch = true; 
};


#endif



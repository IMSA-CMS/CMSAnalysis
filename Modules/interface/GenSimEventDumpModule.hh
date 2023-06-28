#ifndef GENSIMEVENTDUMPMODULE_HH
#define GENSIMEVENTDUMPMODULE_HH

#include <vector>

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"

// Dumps the contents each event into a text file
class GenSimEventDumpModule : public AnalysisModule
{
public:
  GenSimEventDumpModule(int inumOfEvents = -1);

  virtual bool process() override;

private:
  // prints information of a single event
  void printGenSimParticleCollection(const ParticleCollection<GenSimParticle>& genParts, std::ofstream& my_file) const;

  int getIndexOf(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;

  std::string formatDaughterParticles(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;

  std::string formatMotherParticle(const GenSimParticle& part, const std::vector<GenSimParticle>& genParts) const;

  int numOfEvents;
  int counter;

  bool clearlatch = true; 
};


#endif



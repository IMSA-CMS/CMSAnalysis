#ifndef MIGRATIONMODULE_HH
#define MIGRATIONMODULE_HH

#include "AnalysisModule.hh"

class MatchingModule;

class MigrationModule: public AnalysisModule
{
public:
  MigrationModule(const MatchingModule& matchingModule, int minMass = 300, int maxMass = 3200, int massInterval = 100);
  virtual bool process(const edm::EventBase& event) override;
protected:
  virtual void createHistograms() override;
private:
  const MatchingModule& matching;
  const int minMassCut;
  const int maxMassCut;
  const int interval;

  std::string pickMassBin(double invariantMass);
};

#endif

#ifndef MIGRATIONMODULE_HH
#define MIGRATIONMODULE_HH

#include "HistogramOutputModule.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class LRWeightModule;
class MatchingModule;

// MigrationModule plots # of events over invariant mass
// creates histograms over each mass interval for both gen sim and reco
// gen sim histograms display hard cuts at the end of each mass range

class MigrationModule : public HistogramOutputModule
{
public:
  MigrationModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const LRWeightModule& lrWeightModule, const MatchingModule& matchingModule, int minMass = 300, int maxMass = 3200, int massInterval = 100);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;

private:
  const MatchingModule& matching;
  const int minMassCut;
  const int maxMassCut;
  const int interval;

  // determines which graph the event will be filled in based on gen sim invariant mass
  std::string pickMassBin(double invariantMass);
};

#endif

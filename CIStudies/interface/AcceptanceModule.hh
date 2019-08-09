#ifndef ACCEPTANCEMODULE_HH
#define ACCEPTANCEMODULE_HH

#include "HistogramOutputModule.hh"

class GenSimIdentificationModule;
class MatchingModule;

// AcceptanceModule fills a histogram that contains all of the gen sim particles
// and another that contains the reco particles that have been matched 
// to calculate the acceptance of reconstruction at the end
class AcceptanceModule : public HistogramOutputModule
{
public:
  AcceptanceModule(const GenSimIdentificationModule& genSimModule, const MatchingModule& matchingModule, 
		   int minMass = 300, int maxMass = 3200, int massInterval = 100);

  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

private:
  const GenSimIdentificationModule& genSim;
  const MatchingModule& matching;
  int minMassCut;
  int maxMassCut;
  int interval;
};

#endif

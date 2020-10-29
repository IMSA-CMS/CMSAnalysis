/*
#ifndef WEAKALIGNMENTBIASMODULE_HH
#define WEAKALIGNMENTBIASMODULE_HH

#include "HistogramOutputModule.hh"

namespace reco
{
  class Candidate;
}

class MatchingModule;
class ParticleCollection;

// WeakAlignmentBiasModule is used to create T plots in response to weak alignment bias in the detector

class WeakAlignmentBiasModule : public HistogramOutputModule
{
public:
  WeakAlignmentBiasModule(const MatchingModule& matchingModule, int minMass = 1000, int maxMass = 2400, int massInterval = 20);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  // virtual void finalize() override {};

private:
  const MatchingModule& matching;
  const int minMassCut;
  const int maxMassCut;
  const int interval;

  double changePtUp(double pt, double kappa) const {return (pt/(1-(pt*kappa)));}
  double changePtDown(double pt, double kappa) const {return (pt/(1+(pt*kappa)));}
  // double calculateRelativeDifference(double scaled, double original) const {return ((scaled-original)/original);}
  double getScaledInvariantMass(const ParticleCollection& bestParticles, bool scaleUp, bool scaleUpDown) const;
  double calculateScaledInvariantMass(const reco::Candidate* particle1, const reco::Candidate* particle2, double scaledPt1, double scaledPt2) const;

  // determines which graph the event will be filled in based on gen sim invariant mass
  std::string pickMassBin(double invariantMass);
};

#endif

*/

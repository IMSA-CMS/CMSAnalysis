#ifndef SCALEDMASSHIST_HH
#define SCALEDMASSHIST_HH

#include <string>
#include "HistogramPrototype1D.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

namespace reco
{
  class Candidate;
}

class MatchingModule;


class ScaledMassHist : public HistogramPrototype1D
{
public:
  ScaledMassHist(const MatchingModule& matchingModule, const std::string& iname, int nBins, int minMass, int maxMass, bool scaledUp, bool scaledUpDown, bool typeSwitch);
  std::vector<double> value() const override;
  virtual ~ScaledMassHist() {}

private:
  const MatchingModule& matching;
  
  bool scaleUp;
  bool scaleUpDown;
  bool isGenSim;

  double changePtUp(double pt, double kappa) const {return (pt/(1-(pt*kappa)));}
  double changePtDown(double pt, double kappa) const {return (pt/(1+(pt*kappa)));}

  double getScaledInvariantMass(const ParticleCollection<Particle>& bestParticles, bool scaleUp, bool scaleUpDown) const;
  //double getScaledInvariantMass(const ParticleCollection<GenSimParticle>& bestParticles, bool scaleUp, bool scaleUpDown) const;
  double calculateScaledInvariantMass(const Particle particle1, const Particle particle2, double scaledPt1, double scaledPt2) const;

  
};



#endif

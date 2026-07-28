#ifndef TWOINVARIANTMASSESHIST_HH
#define TWOINVARIANTMASSESHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype2D.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"

class TwoInvariantMassesHist : public HistogramPrototype2D
{
  public:
  TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY,
    bool useCorrectedLeptons = false);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;
  private:
    bool useCorrectedLeptons;
    // Sorts two particle pairs based on flavor, charge, and invariant mass
    // Returns the sorted pair of invariant masses in the correct order
    std::pair<double, double> sortParticlePairs(const std::pair<Particle, Particle>& firstPair, const std::pair<Particle, Particle>& secondPair) const;

    // Gets all corrected leptons
    ParticleCollection<Lepton> getCorrectedLeptons() const;
};

#endif

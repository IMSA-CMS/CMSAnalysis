#ifndef TWOINVARIANTMASSESHIST_HH
#define TWOINVARIANTMASSESHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype2D.hh"

class TwoInvariantMassesHist : public HistogramPrototype2D
{
  public:
  TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;
  private:
    // Sorts two particle pairs based on flavor, charge, and invariant mass
    // Returns the sorted pair of invariant masses in the correct order
    std::pair<double, double> sortParticlePairs(const std::pair<Particle, Particle>& firstPair, const std::pair<Particle, Particle>& secondPair) const;
};

#endif

#ifndef MATCHEDRECOINVARIANTMASSHIST_HH
#define MATCHEDRECOINVARIANTMASSHIST_HH

#include "HistogramPrototype.hh"

class MatchingModule;
class GenSimIdentificationModule;
class Particle;

class MatchedRecoInvariantMassHist : public HistogramPrototype
{
  public:
    MatchedRecoInvariantMassHist(const std::shared_ptr<MatchingModule> iMatchMod, const std::shared_ptr<GenSimIdentificationModule> iGenSimMod, const double iHiggsMass, const double iWidth, const std::string& iName, int iNBins, double iMin, double iMax);
    double value() const override;

  private:
    const std::shared_ptr<MatchingModule> matchMod;
    const std::shared_ptr<GenSimIdentificationModule> genSimMod;
    const double higgsMass;
    const double width;
};

#endif

#ifndef RESOLUTIONMODULE_HH
#define RESOLUTIONMODULE_HH

#include "HistogramOutputModule.hh"
#include <TGraphErrors.h>
#include <vector>
#include <memory>

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class MatchingModule;
class MatchingPairCollection;

class ResolutionModule : public HistogramOutputModule
{
public:
  ResolutionModule(const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule, const std::shared_ptr<MatchingModule> matchingModule, std::string bin, int min, int max, int intervalSize);
  virtual bool process() override;
  virtual void initialize() override;
  virtual void finalize() override;

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) = 0;
  std::string pickBin(double value) const;

private:
  const std::shared_ptr<MatchingModule> matching;
  const std::string binType;
  const int minCut;
  const int maxCut;
  const int interval;

  const double minError = -0.5;
  const double maxError = 0.5;

  const int getNumberOfBins() const;
  TGraphErrors* makeTGraphErrors(std::string name, int numberOfBins);

  std::pair<double, double> getErrorValues(TH1* histogram, std::string errorType);
};

#endif

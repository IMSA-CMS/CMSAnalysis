#ifndef RESOLUTIONMODULE_HH
#define RESOLUTIONMODULE_HH

#include "HistogramOutputModule.hh"
#include <TGraphErrors.h>
#include <vector>

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class MatchingModule;
class MatchingPairCollection;

class ResolutionModule : public HistogramOutputModule
{
public:
  ResolutionModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const MatchingModule& matchingModule, std::string bin, int min, int max, int intervalSize);
  virtual bool process(const edm::EventBase& event) override;
  virtual void initialize() override;
  virtual void finalize() override;

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) = 0;
  std::string pickBin(double value) const;

private:
  const MatchingModule& matching;
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

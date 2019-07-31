#ifndef RESOLUTIONMODULE_HH
#define RESOLUTIONMODULE_HH

#include "AnalysisModule.hh"
#include <TGraphErrors.h>
#include <vector>

class MatchingModule;

class ResolutionModule : public AnalysisModule
{
public:
  ResolutionModule(const MatchingModule& matchingModule, int minPt = 50, int maxPt = 1900, int pTInterval = 50);
  virtual bool process(const edm::EventBase& event) override;
  virtual void initialize() override;
  virtual void finalize() override;

private:
  const MatchingModule& matching;
  const int minPtCut;
  const int maxPtCut;
  const int interval;

  std::string pickPtBin(double pT) const;
  const int getNumberOfBins() const;
  TGraphErrors* makeTGraphErrors(std::string name, int numberOfBins);

  std::pair<double, double> getErrorValues(TH1* histogram, std::string errorType);
};

#endif

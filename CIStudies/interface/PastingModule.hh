#ifndef PASTINGMODULE_HH
#define PASTINGMODULE_HH

#include "HistogramOutputModule.hh"
#include <unordered_map>
#include <vector>

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class LRWeightModule;
class PtResolutionModule;
class HistogramPrototype;

class PastingModule : public HistogramOutputModule
{

public:

  PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const LRWeightModule& lrWeightModule);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;
  void addHistogram(HistogramPrototype* hist) {histograms.push_back(hist);}; // Adds a HistogramPrototype* to histogram (the vector)

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  const WeightingModule& weighting;
  const LRWeightModule & lrWeighting;
  // const PtResolutionModule& ptRes;
  // const int histBins = 54;
  // const int minMassCut;
  // const int maxMassCut;
  // const int minPtCut;
  // const int maxPtCut;
  std::unordered_map<std::string, double> massBins;
  std::unordered_map<std::string, std::string> fileKeys;

  bool isNewMassBin(const std::string mass);
  std::vector<HistogramPrototype*> histograms;

};


#endif

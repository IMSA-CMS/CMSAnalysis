#ifndef PASTINGMODULE_HH
#define PASTINGMODULE_HH

#include "HistogramOutputModule.hh"
#include "BigPastingModule.hh"
#include <unordered_map>
#include <vector>

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class LRWeightModule;
class PtResolutionModule;
//class HistogramPrototype;

class PastingModule : public HistogramOutputModule
{

friend class BigPastingModule;

public:
  PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const PtResolutionModule& ptResModule, const LRWeightModule& lrWeightModule, int minMass = 300, int maxMass = 5000, int minPt = 50, int maxPt = 1900);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;
  void addHistograms(HistogramPrototype* hist) {histograms.push_back(hist);}; //Adds a HistogramPrototype* object to the vector named histogram

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  const WeightingModule& weighting;
  const PtResolutionModule& ptRes;
  const LRWeightModule& lrWeighting;
  const int histBins = 94;
  const int minMassCut;
  const int maxMassCut;
  const int minPtCut;
  const int maxPtCut;
  std::unordered_map<std::string, double> massBins;
  std::unordered_map<std::string, std::string> fileKeys;

  bool isNewMassBin(const std::string mass);
  std::vector<HistogramPrototype*> histograms;

};


#endif

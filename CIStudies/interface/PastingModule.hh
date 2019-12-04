#ifndef PASTINGMODULE_HH
#define PASTINGMODULE_HH

#include "HistogramOutputModule.hh"
#include <unordered_map>

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;

class PastingModule : public HistogramOutputModule
{
public:
  PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, int minMass = 300, int maxMass = 5000);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  const WeightingModule& weighting;
  const int histBins = 94;
  const int minMassCut;
  const int maxMassCut;
  std::unordered_map<std::string, double> massBins;
  std::unordered_map<std::string, std::string> fileKeys;

  bool isNewMassBin(const std::string mass);
};


#endif

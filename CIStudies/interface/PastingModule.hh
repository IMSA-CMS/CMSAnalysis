#ifndef PASTINGMODULE_HH
#define PASTINGMODULE_HH

#include "HistogramOutputModule.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;

class PastingModule : public HistogramOutputModule
{
public:
  PastingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, int minMass = 300, int maxMass = 3200, int massInterval = 100);
  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  const int histBins = 100;
  const int minMassCut;
  const int maxMassCut;
  const int interval;
  std::vector<std::string> massBins;

  bool isNewMassBin(const std::string mass);
};


#endif

#ifndef AFBMODULE_HH
#define AFBMODULE_HH

#include "HistogramOutputModule.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;

class AFBModule : public HistogramOutputModule
{
public:
  AFBModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, int minMass = 300, int maxMass = 3200, int massInterval = 100);

  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  int minMassCut;
  int maxMassCut;
  int interval;

  const int histBins = 100;

  void fillFB(std::string level, double collinsSoper, double invariantMass);
  double getAFB(std::string level, int binNumber);
};

#endif

#ifndef UNMATCHEDPARTICLEMODULE_HH
#define UNMATCHEDPARTICLEMODULE_HH

#include "HistogramOutputModule.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;
class WeightingModule;
class LRWeightModule;
class MatchingModule;

class UnmatchedParticleModule : public HistogramOutputModule
{
public:
  UnmatchedParticleModule(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeighting, const std::shared_ptr<MatchingModule> matchingModule);

  virtual void initialize() override;
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() {AnalysisModule::finalize();}

private:
  const std::shared_ptr<GenSimIdentificationModule> genSim;
  const std::shared_ptr<MatchingModule> matching;

  const int histBins = 100;
};

#endif

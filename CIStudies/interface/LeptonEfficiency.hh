#ifndef LEPTONEFFICIENCY_HH
#define LEPTONEFFICIENCY_HH

#include "AnalysisModule.hh"

class MatchingModule;
class GenSimIdentificationModule;

class LeptonEfficiency : public AnalysisModule
{
public:
  LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule, const std::shared_ptr<GenSimIdentificationModule> igenSimModule);
  bool process(const edm::EventBase& event) override;
  void finalize() override;
  void writeAll() override {}
private:
  const std::shared_ptr<MatchingModule> matchModule;
  const std::shared_ptr<GenSimIdentificationModule> genSimModule;
  int recoMuons, genSimMuons, recoElectrons, genSimElectrons;
};

#endif

#ifndef LEPTONEFFICIENCY_HH
#define LEPTONEFFICIENCY_HH

#include "AnalysisModule.hh"

class MatchingModule;
class GenSimIdentificationModule;

class LeptonEfficiency : public AnalysisModule
{
public:
  LeptonEfficiency(const MatchingModule& imatchModule, const GenSimIdentificationModule& igenSimModule);
  bool process(const edm::EventBase& event) override;
  void finalize() override;
  void writeAll() override {}
private:
  const MatchingModule& matchModule;
  const GenSimIdentificationModule& genSimModule;
  int recoMuons, genSimMuons, recoElectrons, genSimElectrons;
};

#endif

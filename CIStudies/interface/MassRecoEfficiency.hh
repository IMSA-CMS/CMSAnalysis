#ifndef MASSRECOEFFICIENCY_HH
#define MASSRECOEFFICIENCY_HH

#include "AnalysisModule.hh"

class RecoIdentificationModule;

class MassRecoEfficiency : public AnalysisModule
{
public:
  MassRecoEfficiency(const RecoIdentificationModule& iRecoModule, double iHiggsMass, double iWidth);
  bool process(const edm::EventBase& event) override;
  void finalize() override;
  void writeAll() override {}
private:
  const RecoIdentificationModule& recoModule;
  double HiggsMass;
  double width;     // Checks HiggsMass +/- width
  int passCount;
  int totalCount;
  std::unordered_map<int, int> nLeptonPassCount;
  std::unordered_map<int, int> nLeptonTotalCount;
};

#endif

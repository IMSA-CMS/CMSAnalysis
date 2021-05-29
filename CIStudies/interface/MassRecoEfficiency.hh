#ifndef MASSRECOEFFICIENCY_HH
#define MASSRECOEFFICIENCY_HH

#include <memory>
#include "AnalysisModule.hh"

class RecoIdentificationModule;

class MassRecoEfficiency : public AnalysisModule
{
public:
  MassRecoEfficiency(const std::shared_ptr<RecoIdentificationModule> iRecoModule, double iHiggsMass, double iLowerWidth, double iUpperWidth);
  bool process(const edm::EventBase& event) override;
  void finalize() override;
  void writeAll() override {}
private:
  const std::shared_ptr<RecoIdentificationModule> recoModule;
  double HiggsMass;
  double lowerWidth;     // Checks HiggsMass - lowerWidth, HiggsMass + higherWidth
  double upperWidth;
  int passCount;
  int totalCount;
  std::map<int, int> nLeptonPassCount;
  std::map<int, int> nLeptonTotalCount;
  std::map<int, std::map<int, int>> muonsNLeptonPassCount;   // NLeptons keys to <NMuons, count> 
  std::map<int, std::map<int, int>> muonsNLeptonTotalCount;
};

#endif

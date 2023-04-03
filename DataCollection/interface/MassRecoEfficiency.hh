#ifndef MASSRECOEFFICIENCY_HH
#define MASSRECOEFFICIENCY_HH

#include <memory>
#include "EfficiencyModule.hh"


class MassRecoEfficiency : public EfficiencyModule
{
public:
  MassRecoEfficiency(double iHiggsMass, double iLowerWidth, double iUpperWidth);
  bool process() override;
  void finalize() override;
  void writeAll() override {}

private:
  double HiggsMass;
  double lowerWidth;     // Checks HiggsMass - lowerWidth, HiggsMass + higherWidth
  double upperWidth;
  int passCount = 0;
  int totalCount = 0;
  std::map<int, int> nLeptonPassCount;
  std::map<int, int> nLeptonTotalCount;
  std::map<int, std::map<int, int>> muonsNLeptonPassCount;   // NLeptons keys to <NMuons, count> 
  std::map<int, std::map<int, int>> muonsNLeptonTotalCount;
  int posPassCount = 0;
  int posTotalCount = 0;
  int negPassCount = 0;
  int negTotalCount = 0;

  protected:
  void doCounters() override {}
};

#endif

#ifndef TRIGGEREFFICIENCYMODULE_HH
#define TRIGGEREFFICIENCYMODULE_HH

#include "AnalysisModule.hh"

#include <memory>

class MatchingModule;
//class GenSimIdentificationModule;
class Particle;

class TriggerEfficiencyModule : public AnalysisModule
{
  public:
    TriggerEfficiencyModule(const std::shared_ptr<MatchingModule> iMatchMod, double iHiggsMass, double iLowerWidth, double iUpperWidth);
    bool process() override;
    void finalize() override;
    void writeAll() override {};
    void printDebugLines(const Particle particle) const;
  private:
    const std::shared_ptr<MatchingModule> matchMod;
    //const std::shared_ptr<GenSimIdentificationModule> genSimMod;

    double higgsMass;
    double lowerWidth;
    double upperWidth;

    int recoCount = 0;
    int genSimCount = 0;
    int expectedHiggsCount = 0;
};

#endif

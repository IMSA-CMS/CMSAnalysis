#ifndef TRIGGEREFFICIENCYMODULE_HH
#define TRIGGEREFFICIENCYMODULE_HH

#include "AnalysisModule.hh"

#include <memory>

class MatchingModule;
class GenSimIdentificationModule;

class TriggerEfficiencyModule : public AnalysisModule
{
  public:
    TriggerEfficiencyModule(const std::shared_ptr<MatchingModule> iMatchMod, const std::shared_ptr<GenSimIdentificationModule> iGenSimMod, double iHiggsMass, double iWidth);
    bool process(const edm::EventBase& event) override;
    void finalize() override;
  private:
    const std::shared_ptr<MatchingModule> matchMod;
    const std::shared_ptr<GenSimIdentificationModule> genSimMod;

    double higgsMass;
    double width;

    int recoCount = 0;
    int genSimCount = 0;
};

#endif

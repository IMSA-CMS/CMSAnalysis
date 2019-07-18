#ifndef ACCEPTANCEMODULE_HH
#define ACCEPTANCEMODULE_HH

#include "AnalysisModule.hh"

class AcceptanceModule : public AnalysisModule
{
public:
  virtual bool process(const edm::EventBase& event) override;
protected:
  virtual void createHistograms() override;
private:
  int minMassCut;
  int maxMassCut;
};

#endif

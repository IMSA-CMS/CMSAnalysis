#ifndef ACCEPTANCEXMIGRATIONMODULE_HH
#define ACCEPTANCEXMIGRATIONMODULE_HH

#include "AnalysisModule.hh"

class CommonValuesModule;

class AcceptanceXMigrationModule: public AnalysisModule
{
public:
  AcceptanceXMigrationModule(const CommonValuesModule& commonModule, int minMass = 300, int maxMass = 3100, int massInterval = 100);
  virtual bool process(const edm::EventBase& event) override;
protected:
  virtual void createHistograms() override;
private:
  const CommonValuesModule& common;
  const int minMassCut;
  const int maxMassCut;
  const int interval;

  std::string pickMassBin(double invariantMass);
};

#endif

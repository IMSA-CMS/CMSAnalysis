#ifndef SIGNFLIPMODULE_HH
#define SIGNFLIPMODULE_HH

#include "AnalysisModule.hh"

class MatchingModule;
class GenSimIdentificationModule;

class SignFlipModule : public AnalysisModule
{
public:
  SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule);
  bool process(const edm::EventBase& event) override;
  void finalize() override;
  void writeAll() override {}
private:
  const std::shared_ptr<MatchingModule> matchModule;
  int nSignFlips = 0;
  int nEvents = 0;
};

#endif

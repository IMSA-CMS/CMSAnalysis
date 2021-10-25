#ifndef SIGNFLIPMODULE_HH
#define SIGNFLIPMODULE_HH

#include "AnalysisModule.hh"

class MatchingModule;
class GenSimIdentificationModule;

class SignFlipModule : public AnalysisModule
{
public:
  SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule);
  bool process() override;
  void finalize() override;
  void writeAll() override {}
private:
  const std::shared_ptr<MatchingModule> matchModule;
  int nSignFlips = 0;
  int nElectronFlips = 0;
  int nElectronEvents = 0;
  int nMuonFlips = 0;
  int nMuonEvents = 0;
  int nTotalEvents = 0;

  // Define a pT cut to keep track of low- vs. high-pT events
  // This will also define a few more variables to keep track
  // of high-pT events.
  double signFlipPtCut = 120;
  int nHighPtElectronEvents = 0;
  int nHighPtElectronFlips = 0;
  int nHighPtMuonEvents = 0;
  int nHighPtMuonFlips = 0;
};

#endif

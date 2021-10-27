#ifndef HIGGSEFFICIENCY_HH
#define HIGGSEFFICIENCY_HH

#include "AnalysisModule.hh"

class MatchingModule;
class GenSimIdentificationModule;

class HiggsEfficiency : public AnalysisModule
{
  public:
    bool MatchPair(); // Function to check if lepton type and charge are the same (from GenSim)
  private:
    const MatchingModule& matchMod;
    const GenSimIdentificationModule& genSimMod;
};



#endif


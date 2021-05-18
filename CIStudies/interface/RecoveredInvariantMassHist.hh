#ifndef RECOVEREDINVARIANTMASSHIST_HH
#define RECOVEREDINVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class RecoveredInvariantMassHist : public GenSimRecoPrototype
{
  public:
    RecoveredInvariantMassHist(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons);
  protected:
    double protectedValue(bool typeGenSim) const override;
  private:
    int nLeptons;  // # of leptons to be calculated in calculateRecoveredInvariantMass() function
};

#endif

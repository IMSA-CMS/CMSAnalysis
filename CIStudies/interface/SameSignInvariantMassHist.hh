#ifndef SAMESIGNINVARIANTMASSHIST_HH
#define SAMESIGNINVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class GenSimIdentificationModule;
class RecoIdentificationModule;

class SameSignInvariantMassHist : public GenSimRecoPrototype
{
  public:
    SameSignInvariantMassHist(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, bool iUsingPhi = false, bool iMultipleMasses = false);
  protected:
    virtual std::vector<double> protectedValue(bool typeGenSim) const override;
  private:
    bool usingPhi;
    bool multipleMasses;
};

#endif

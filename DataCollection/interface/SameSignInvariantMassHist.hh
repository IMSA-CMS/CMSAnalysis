#ifndef SAMESIGNINVARIANTMASSHIST_HH
#define SAMESIGNINVARIANTMASSHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class SameSignInvariantMassHist : public GenSimRecoPrototype
{
  public:
    SameSignInvariantMassHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, bool iUsingPhi = false, 
      bool iMultipleMasses = false);
  protected:
    virtual std::vector<double> protectedValue(InputModule::RecoLevel typeGenSim) const override;
  private:
    bool usingPhi;
    bool multipleMasses;
};

#endif

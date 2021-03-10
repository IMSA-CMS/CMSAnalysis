#ifndef GETNTHHIGHESTPTHIST_HH
#define GETNTHHIGHESTPTHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class GetNthHighestPtHist : public GenSimRecoPrototype
{
  public:
    GetNthHighestPtHist(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon = 1);
    using GenSimRecoPrototype::GenSimRecoPrototype;
    

  protected:
    double protectedValue(bool typeGenSim) const override;

  private:
    int nthMuon;
};

#endif

#ifndef GETNTHHIGHESTPTHIST_HH
#define GETNTHHIGHESTPTHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class GetNthHighestPtHist : public GenSimRecoPrototype
{
  public:
    GetNthHighestPtHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon = 1);
    using GenSimRecoPrototype::GenSimRecoPrototype;
    

  protected:
    std::vector<double> protectedValue(InputModule::RecoLevel typeGenSim) const override;

  private:
    int nthMuon;
};

#endif

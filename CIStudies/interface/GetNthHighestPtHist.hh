#ifndef GETNTHHIGHESTPTHIST_HH
#define GETNTHHIGHESTPTHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class GetNthHighestPtHist : public GenSimRecoPrototype
{
  public:
    GetNthHighestPtHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon = 1);
    using GenSimRecoPrototype::GenSimRecoPrototype;
    

  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;

  private:
    int nthMuon;
};

#endif

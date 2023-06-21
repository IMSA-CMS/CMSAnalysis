#ifndef PHOTONELECTRONINVARIANTMASSHIST_HH
#define PHOTONELECTRONINVARIANTMASSHIST_HH

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"

class PhotonElectronInvariantMassHist : public GenSimRecoPrototype
{
  public:
    PhotonElectronInvariantMassHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum);
  protected:
    std::vector<double> protectedValue(InputModule::RecoLevel level) const override;
};

#endif

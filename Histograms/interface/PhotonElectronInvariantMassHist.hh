#ifndef PHOTONELECTRONINVARIANTMASSHIST_HH
#define PHOTONELECTRONINVARIANTMASSHIST_HH

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"

class PhotonElectronInvariantMassHist : public GenSimRecoPrototype
{
  public:
    PhotonElectronInvariantMassHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum);
  protected:
    std::vector<double> protectedValue(EventInput::RecoLevel level) const override;
};

#endif

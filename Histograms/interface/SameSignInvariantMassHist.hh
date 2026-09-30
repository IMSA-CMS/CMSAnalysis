#ifndef SAMESIGNINVARIANTMASSHIST_HH
#define SAMESIGNINVARIANTMASSHIST_HH

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"

class SameSignInvariantMassHist : public GenSimRecoPrototype
{
  public:
    SameSignInvariantMassHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, bool useCorrectedLeptons = false);
  protected:
    virtual std::vector<double> protectedValue(EventInput::RecoLevel typeGenSim) const override;
  private:
    bool corrected;
    ParticleCollection<Lepton> getCorrectedLeptons() const;
};

#endif

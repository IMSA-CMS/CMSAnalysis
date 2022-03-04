#ifndef RECOVEREDINVARIANTMASSHIST_HH
#define RECOVEREDINVARIANTMASSHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class RecoveredInvariantMassHist : public GenSimRecoPrototype
{
  public:
    RecoveredInvariantMassHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons, int iMotherPDGID);
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
  private:
    int nLeptons;  // # of leptons to be calculated in calculateRecoveredInvariantMass() function
    int motherPDGID;  // the PDGID of the mother particle
};

#endif
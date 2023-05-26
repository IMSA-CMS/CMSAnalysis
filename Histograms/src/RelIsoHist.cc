#include "CMSAnalysis/Histograms/interface/RelIsoHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include <vector>

RelIsoHist::RelIsoHist(const std::string &iname, int iNBins, double iminimum, double imaximum) : HistogramPrototype1D(iname, iNBins, iminimum, imaximum){};

std::vector<double> RelIsoHist::value() const
{
  std::vector<double> output;
  for (auto particle : getInput()->getLeptons(InputModule::RecoLevel::Reco))
  {
    double isolation = particle.getIsolation();
    output.push_back(isolation);
  }
  return output;
};

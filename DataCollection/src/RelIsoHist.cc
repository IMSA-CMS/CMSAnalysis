#include "CMSAnalysis/DataCollection/interface/RelIsoHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

#include <vector>

RelIsoHist::RelIsoHist(const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
{
}

std::vector<double> RelIsoHist::value() const
{
  std::vector<double> output;
  for (auto particle : getInput()->getLeptons(InputModule::RecoLevel::Reco)) {
    double isolation = particle.getIsolation();
    output.push_back(isolation);
  }
  return output;
}

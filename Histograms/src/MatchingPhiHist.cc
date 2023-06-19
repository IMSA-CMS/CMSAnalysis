#include "CMSAnalysis/Histograms/interface/MatchingPhiHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include <vector>

MatchingPhiHist::MatchingPhiHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonJetMatchingModule(iLeptonJetMatchingModule)

{}

std::vector<double> MatchingPhiHist::value() const
{
    const auto& matchingPairs = leptonJetMatchingModule->getMatchingPairs();
    if (matchingPairs.size() != 0)
    {
        // std::cout << "MatchingPairs size not zero" << "\n";
        auto pair = matchingPairs[0];
        auto partPhi = pair.first.getFourVector().Phi();
        // std::cout << "MC phi value: " << partPhi << "\n";
        auto jetPhi = pair.second.getFourVector().Phi();
        // std::cout << "Reco phi value: " << jetPhi << "\n";
        double deltaPhi = reco::deltaPhi(partPhi, jetPhi);

        return {deltaPhi};
    }
    else
    {
        return {-1};
    }
}

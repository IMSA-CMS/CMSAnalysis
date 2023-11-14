#include "CMSAnalysis/Histograms/interface/MatchingPtHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include <vector>

MatchingPtHist::MatchingPtHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonJetMatchingModule(iLeptonJetMatchingModule)

{}

std::vector<double> MatchingPtHist::value() const
{
    const auto& matchingPairs = leptonJetMatchingModule->getMatchingPairs();
    if (matchingPairs.size() != 0)
    {
        // std::cout << "MatchingPairs size not zero" << "\n";
        auto pair = matchingPairs[0];
        double partPt = pair.first.getPt();
        double jetPt = pair.second.getPt();

        double ptDiff = jetPt - partPt;
        std::cout << "Matching pair pT: " << ptDiff << "\n";

        return {ptDiff};
    }
    else
    {
        return {-999};
    }
}

#include "CIAnalysis/CIStudies/interface/MatchingDeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>

MatchingDeltaRHist::MatchingDeltaRHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
    HistogramPrototype(iname, iNBins, iminimum, imaximum),
    leptonJetMatchingModule(iLeptonJetMatchingModule)

{}

double MatchingDeltaRHist::value() const
{
    const auto& matchingPairs = leptonJetMatchingModule->getMatchingPairs();
    if (matchingPairs.size() != 0)
    {
        std::cout << "MatchingPairs size not zero" << "\n";
        auto pair = matchingPairs[0];
        auto partFourVector = pair.first.fourVector();
        auto jetFourVector = pair.second.getFourVector();
        double deltaR = reco::deltaR(partFourVector, jetFourVector);

        std::cout << "Matching Pair Delta R: " << deltaR << "\n";

        return deltaR;
    }
    else
    {
        return -1;
    }
}

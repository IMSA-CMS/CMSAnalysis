#include "CIAnalysis/CIStudies/interface/MatchingEtaHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

#include <vector>

MatchingEtaHist::MatchingEtaHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonJetMatchingModule(iLeptonJetMatchingModule)

{}

std::vector<double> MatchingEtaHist::value() const
{
    const auto& matchingPairs = leptonJetMatchingModule->getMatchingPairs();
    if (matchingPairs.size() != 0)
    {
        // std::cout << "MatchingPairs size not zero" << "\n";
        auto pair = matchingPairs[0];
        double partEta = pair.first.eta();
        std::cout << "MC eta value: " << partEta << "\n";
        double jetEta = pair.second.getEta();
        std::cout << "Reco eta value: " << jetEta << "\n";

        double etaDiff = jetEta - partEta;

        return {etaDiff};
    }
    else
    {
        return {-999};
    }
}

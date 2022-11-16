// Does nothing right now

#include "CMSAnalysis/DataCollection/interface/MatchedLeptonJetHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

MatchedLeptonJetHist::MatchedLeptonJetHist(const std::string& iname, int inBins, double iminimum, double imaximum,
    std::shared_ptr<LeptonJetMatchingModule> ilepMatchMod,
    std::shared_ptr<LeptonJetReconstructionModule> ilepRecMod,
    bool iMatched) :
  HistogramPrototype1D(iname, inBins, iminimum, imaximum),
  lepMatchMod(ilepMatchMod),
  lepRecMod(ilepRecMod),
  matched(iMatched){}

std::vector<double> MatchedLeptonJetHist::value() const
{
  const auto &matchingPairs = lepMatchMod->getMatchingPairs();
  const auto &recoJets = lepRecMod->getLeptonJets();
  std::vector<double> out;
  /*
  if (recoJets.size() != matchingPairs.size()) {
    std::cout << recoJets.size() << " " << matchingPairs.size() << "\n";
  }
  */

  Particle nullParticle(Particle::nullParticle());
  LeptonJet nullJet;

  for (unsigned int i = 0; i < recoJets.size(); i++) {
    const auto lj = recoJets[i];
    const auto lj4 = lj.getFourVector();
    double min = std::numeric_limits<double>::max();
    auto candidate = nullJet;
    for (unsigned int j = 0; j < matchingPairs.size(); j++) {
      const auto mp = matchingPairs[j];
      const auto mlj = mp.second;
      const auto mlj4 = mlj.getFourVector();
      const auto dr = reco::deltaR(lj4, mlj4);
      if (dr < min) {
        min = dr;
      }
    }
    if (min > 0 && ((min < 0.1) == matched)) {
      // out.push_back(lj.getParticles()[0].hadVetoEt() / lj4.pt());
    }
  }

  return out;
}


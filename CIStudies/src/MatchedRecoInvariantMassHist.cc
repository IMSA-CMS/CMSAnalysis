#include "CIAnalysis/CIStudies/interface/MatchedRecoInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"

MatchedRecoInvariantMassHist::MatchedRecoInvariantMassHist(const std::shared_ptr<MatchingModule> iMatchMod, const std::shared_ptr<GenSimIdentificationModule> iGenSimMod, const double iHiggsMass, const double iWidth, const std::string& iName, int iNBins, double iMin, double iMax) : 
  HistogramPrototype(iName, iNBins, iMin, iMax),
  matchMod(iMatchMod),
  genSimMod(iGenSimMod),
  higgsMass(iHiggsMass),
  width(iWidth)
{
}

double MatchedRecoInvariantMassHist::value() const
{
  auto genSimParticles = genSimMod->getGenParticles().getParticles();  // Vector of Particles

  for (size_t i = 0; i < genSimParticles.size(); ++i)
  {
    for (size_t j = i + 1; j < genSimParticles.size(); ++j)  // As to avoid overcounting
    {
      auto genSimParticle1 = genSimParticles[i];
      auto genSimParticle2 = genSimParticles[j];

      if ((genSimParticle1 != genSimParticle2) && (genSimParticle1.uniqueMother() == genSimParticle2.uniqueMother()) && (genSimParticle1.charge() == genSimParticle2.charge()) && genSimParticle1.isNotNull() && genSimParticle2.isNotNull())
      {
        ParticleCollection genSimPair;
        genSimPair.addParticle(genSimParticle1);
        genSimPair.addParticle(genSimParticle2);

        if (((higgsMass - width) <= genSimPair.calculateSameSignInvariantMass()) && (genSimPair.calculateSameSignInvariantMass() <= (higgsMass + width)))
        {
          ParticleCollection matchedReco;  // ParticleCollection that we will add the matched Reco particles to in a bit

          auto matchedPairs = matchMod->getMatchingBestPairs().getPairs();  // This is a vector of Matched Pairs

          for (auto pair : matchedPairs)
          {
            if (((genSimParticle1 == pair.getGenParticle()) || (genSimParticle2 == pair.getGenParticle())) && pair.getGenParticle().isNotNull())
            {
              matchedReco.addParticle(pair.getRecoParticle());
            }
          }

          if (((higgsMass - width) <= matchedReco.calculateSameSignInvariantMass()) && (matchedReco.calculateSameSignInvariantMass() <= (higgsMass + width)))
          {
            return matchedReco.calculateSameSignInvariantMass();
          }
        }
      }
    }
  }

  return -1;
}

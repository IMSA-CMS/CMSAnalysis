#include "CIAnalysis/CIStudies/interface/TriggerEfficiencyModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

TriggerEfficiencyModule::TriggerEfficiencyModule(const std::shared_ptr<MatchingModule> iMatchMod, const std::shared_ptr<GenSimIdentificationModule> iGenSimMod, double iHiggsMass, double iWidth) :
  matchMod(iMatchMod),
  genSimMod(iGenSimMod),
  higgsMass(iHiggsMass),
  width(iWidth)
{
}

bool TriggerEfficiencyModule::process(const edm::EventBase& event)
{
  auto genSimParticles = genSimMod->getGenParticles().getParticles();

  for (auto genSimParticle1 : genSimParticles)
  {
    for (auto genSimParticle2 : genSimParticles)
    {
      if ((genSimParticle1 != genSimParticle2) && (genSimParticle1.mother() == genSimParticle2.mother()) && (genSimParticle1.charge() == genSimParticle2.charge()))
      {
        auto genSimPair = new ParticleCollection();
        genSimPair->addParticle(genSimParticle1);
        genSimPair->addParticle(genSimParticle2);
                
        if (((higgsMass - width) <= genSimPair->getInvariantMass()) && (genSimPair->getInvariantMass() <= (higgsMass + width)))
        {
          ++genSimCount;

          auto matchedReco = new ParticleCollection();  // ParticleCollection that we will add the matched Reco particles to in a bit

          auto matchedPairs = matchMod->getMatchingBestPairs().getPairs();  // This is a vector of Matched Pairs

          for (auto pair : matchedPairs)
          {
            if ((genSimParticle1 == pair.getGenParticle()) || (genSimParticle2 == pair.getGenParticle()))
            {
              matchedReco->addParticle(pair.getRecoParticle());
            }
          }

          if (((higgsMass - width) <= matchedReco->getInvariantMass()) && (matchedReco->getInvariantMass() <= (higgsMass + width)))
          {
            ++recoCount;
          }

          matchedReco->clear();
        }
      
        genSimPair->clear();
      }
    }
  }

  return true;
}

void TriggerEfficiencyModule::finalize()
{
  std::cout << "Number of GenSim H++'s that fall into the mass window of the H++: " << genSimCount << '\n';
  std::cout << "Number of Reco H++'s that fall into the mass window of the H++: " << recoCount << '\n';
  std::cout << "Overall Trigger Efficiency for H++: " << static_cast<double>(genSimCount) / recoCount << '\n';
}

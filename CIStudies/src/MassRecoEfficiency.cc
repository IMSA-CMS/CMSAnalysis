#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

MassRecoEfficiency::MassRecoEfficiency(const RecoIdentificationModule& iRecoModule, double iHiggsMass, double iWidth):
  recoModule(iRecoModule),
  HiggsMass(iHiggsMass),
  width(iWidth),
  passCount(0),
  totalCount(0)
{
}

bool MassRecoEfficiency::process(const edm::EventBase& event)
{
  auto reco = recoModule.getRecoCandidates();
  double invMass = reco.calculateSameSignInvariantMass();
  int size = reco.getNumParticles();

  double min = HiggsMass - width;
  double max = HiggsMass + width;

  // If there isn't an element in the unordered map with the key that is equal to the # of leptons make it
  if (nLeptonPassCount.find(size) == nLeptonTotalCount.end())
  {
    nLeptonPassCount.insert({size, 0});
  }
  
  // Same thing here, but with the other unordered map
  if (nLeptonTotalCount.find(size) == nLeptonTotalCount.end())
  {
    nLeptonTotalCount.insert({size, 0});
  }

  if (invMass > min && invMass < max)       // if min < invMass < max; in other words, the invariant mass is in the range we're checking
  {
    ++passCount;                            // Passes, increment the pass count
    ++nLeptonPassCount[size];               // Passes, increment the pass count for the count specific to the number of leptons
    
  }

  ++totalCount;
  ++nLeptonTotalCount[size];

  return true;
}

void MassRecoEfficiency::finalize()
{
  std::cout << "Mass Reconstruction Efficiency: " << passCount / (double) totalCount << std::endl;

  for (auto totalCount : nLeptonTotalCount)
  {
    int pass = nLeptonPassCount[totalCount.first];
    int total = totalCount.second;
    std::cout << "Leptons: " << totalCount.first << "; Accepted: " << pass << "; Total: " << total << std::endl;
    std::cout << "Mass Reconstruction Efficiency for " << totalCount.first << " leptons: " << pass / (double) total << std::endl;
  }
}

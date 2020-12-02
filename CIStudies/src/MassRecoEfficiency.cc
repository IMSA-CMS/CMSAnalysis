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
  int nMuons = reco.getLeptonTypeCount(Particle::LeptonType::Muon);

  double min = HiggsMass - width;
  double max = HiggsMass + width;

  // If there isn't an element in the map with the key that is equal to the # of leptons make it
  if (nLeptonPassCount.find(size) == nLeptonTotalCount.end())
  {
    nLeptonPassCount.insert({size, 0});
  }
  
  // Same thing here, but with the other leptons map
  if (nLeptonTotalCount.find(size) == nLeptonTotalCount.end())
  {
    nLeptonTotalCount.insert({size, 0});
  }

  // Same thing with the muon maps
  if (muonsNLeptonPassCount.find(size) == muonsNLeptonPassCount.end())
  {
    muonsNLeptonPassCount.insert({size, {{nMuons, 0}}});
  }

  if (muonsNLeptonTotalCount.find(size) == muonsNLeptonTotalCount.end())
  {
    muonsNLeptonTotalCount.insert({size, {{nMuons, 0}}});
  }

  if (invMass > min && invMass < max)       // if min < invMass < max; in other words, the invariant mass is in the range we're checking
  {
    ++passCount;                            // Passes, increment the pass count
    ++nLeptonPassCount[size];               // Passes, increment the pass count for the count specific to the number of leptons
    ++muonsNLeptonPassCount[size][nMuons];
  }

  ++totalCount;
  ++nLeptonTotalCount[size];
  ++muonsNLeptonTotalCount[size][nMuons];

  return true;
}

void MassRecoEfficiency::finalize()
{
  std::cout << "Mass Reconstruction Efficiency: " << passCount / (double) totalCount << std::endl;

  std::cout << '\n';  // For sanity's sake

  for (auto totalCount : nLeptonTotalCount)
  {
    int pass = nLeptonPassCount[totalCount.first];
    int total = totalCount.second;
    std::cout << "Leptons: " << totalCount.first << "; Accepted: " << pass << "; Total: " << total << std::endl;
    std::cout << "Mass Reconstruction Efficiency for " << totalCount.first << " leptons: " << pass / (double) total << std::endl;
  }

  std::cout << '\n';  // Again, for sanity's sake

  for (auto muonCountPairs : muonsNLeptonTotalCount)
  {
    for (auto totalCount : muonCountPairs.second)
    {
      int pass = muonsNLeptonPassCount[muonCountPairs.first][totalCount.first];
      int total = totalCount.second;
      std::cout << "Muons: " << totalCount.first << "; Leptons: " << muonCountPairs.first << "; Accepted: " << pass << "; Total: " << total << std::endl;
      std::cout << "Mass Reconstruction Efficiency for " << totalCount.first << " muons out of " << muonCountPairs.first << " muons: " << pass / (double) total << std::endl;
    }  

  std::cout << '\n';  // Yet again, for sanity's sake

  }
}

#include "CMSAnalysis/Modules/interface/MassRecoEfficiency.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

MassRecoEfficiency::MassRecoEfficiency(double iHiggsMass, double iLowerWidth, double iUpperWidth):
  EfficiencyModule(),
  //recoModule(iRecoModule),
  HiggsMass(iHiggsMass),
  lowerWidth(iLowerWidth),
  upperWidth(iUpperWidth)
{
}

bool MassRecoEfficiency::process()
{
  auto reco = getInput()->getLeptons(EventInput::RecoLevel::Reco);
  double invMass = reco.calculateSameSignInvariantMass();
  int size = reco.getNumParticles();
  int nMuons = reco.getLeptonTypeCount(ParticleType::muon());

  auto invMasses = reco.calculateSameSignInvariantMasses();

  double min = HiggsMass - lowerWidth;
  double max = HiggsMass + upperWidth;

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

  if (reco.getNumPosParticles() >= 2)  // More than 2 positive particles (otherwise we won't have an invariant mass)
  {
    ++posTotalCount;

    if (invMasses[0] > min && invMasses[0] < max)
    {
      ++posPassCount;
    }

    if (reco.getNumNegParticles() >= 2)
    {
      ++negTotalCount;
      
      //std::cout << invMasses[1] << '\n';

      if (invMasses[1] > min && invMasses[1] < max)  // If the positive invariant mass is calculated, it is in the 0th index; negative is in the 1st
      {
        ++negPassCount;
      }

      //std::cout << "Negatives Passed: " << negPassCount << '\n';
      //std::cout << "Total Negatives: " << negTotalCount << '\n';
    }
  }

  else
  {
    //std::cout << "# of Positive Particles: " << reco.getNumPosParticles() << '\n';
    //std::cout << "# of Negative Particles: " << reco.getNumNegParticles() << '\n';

    if (reco.getNumNegParticles() >= 2)
    {
      ++negTotalCount;

      //std::cout << invMasses[0] << '\n';

      if (invMasses[0] > min && invMasses[0] < max)  // Since the positive invariant mass is not calculated, negative is in the 0th index (if calculated)
      {
        ++negPassCount;
      }

      //std::cout << "Negatives Passed: " << negPassCount << '\n';
      //std::cout << "Total Negatives: " << negTotalCount << '\n';
    }
  }

  ++totalCount;
  ++nLeptonTotalCount[size];
  ++muonsNLeptonTotalCount[size][nMuons];

  return true;
}

void MassRecoEfficiency::finalize()
{
  std::cerr << "--------------------------------------------------------" << std::endl;
  std::cout << "Mass Reconstruction Efficiency for " << HiggsMass << "-" << lowerWidth << "+" << upperWidth << " GeV: " << passCount / (double) totalCount << std::endl;

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

    std::cout << "\n\n";  // Yet again, for sanity's sake
  }

  std::cout << "Mass Reconstruction Efficiency for H++ for " << HiggsMass << "-" << lowerWidth << "+" << upperWidth << " GeV: " << posPassCount / (double) posTotalCount << std::endl;
  std::cout << "Mass Reconstruction Efficiency for H-- for " << HiggsMass << "-" << lowerWidth << "+" << upperWidth << " GeV: " << negPassCount / (double) negTotalCount << std::endl;

  std::cout << "\n\n\n";  // Once again, for sanity's sake
}

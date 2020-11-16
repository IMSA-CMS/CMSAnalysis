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

  double min = HiggsMass - width;
  double max = HiggsMass + width;

  if (invMass > min && invMass < max)  // if min < invMass < max; in other words, the invariant mass is in the range we're checking
  {
    ++passCount;                       // Passes, increment the pass count
  }

  ++totalCount;

  return true;
}

void MassRecoEfficiency::finalize()
{
  std::cout << "Mass Reconstruction Efficiency: " << passCount / (double) totalCount << std::endl;
}

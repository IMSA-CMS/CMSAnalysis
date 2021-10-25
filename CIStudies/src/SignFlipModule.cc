#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/SignFlipModule.hh"

SignFlipModule::SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule):
  matchModule(imatchModule)
{
}

bool SignFlipModule::process()
{
  const auto matched = matchModule->getMatchingBestPairs().getPairs();

  for(const auto &particlePair : matched)
    {
	  nEvents++;
	  
    auto particleType = particlePair.getGenParticle().getType();

    // This variable is to make sure the pointers don't break the code.
    int trashBin = 0;

    auto eventPointer = &trashBin;
    auto flipPointer = &trashBin;
    if(particleType == Particle::Type::Electron) {
      nElectronEvents++;
    } else if(particleType == Particle::Type::Muon) {
      nMuonEvents++;
    }

	  if(particlePair.getGenParticle().charge() != particlePair.getRecoParticle().charge()) {
      if(particleType == Particle::Type::Electron) {
        nElectronFlips++;
      } else if(particleType == Particle::Type::Muon) {
        nMuonFlips++;
      }
		  nSignFlips++;
	  }
  }
  return true;
}
void SignFlipModule::finalize()
{
  std::cout << "Number of Events: " << nEvents << std::endl;
  std::cout << "Number of Sign Flips: " << nSignFlips << std::endl;
  std::cout << "Electron Sign Efficiency: " << (1 - (double) nElectronFlips / nElectronEvents) << std::endl;
  std::cout << "Muon Sign Efficiency: " << (1 - (double) nMuonFlips / nMuonEvents) << std::endl;
}

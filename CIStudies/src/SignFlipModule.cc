#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/SignFlipModule.hh"

SignFlipModule::SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule):
  matchModule(imatchModule)
{
}

bool SignFlipModule::process(const edm::EventBase& event)
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
      eventPointer = &nElectronEvents;
      flipPointer = &nElectronFlips;
    } else if(particleType == Particle::Type::Muon) {
      eventPointer = &nMuonEvents;
      flipPointer = &nMuonFlips;
    }

	  if(particlePair.getGenParticle().charge() != particlePair.getRecoParticle().charge()) {
      *eventPointer++;
      *flipPointer++;
		  nSignFlips++;
	  }
  }
  return true;
}
void SignFlipModule::finalize()
{
  std::cout << "Number of Events: " << nEvents << std::endl;
  std::cout << "Number of Sign Flips: " << nSignFlips << std::endl;
  std::cout << "Overall Sign Efficiency: " << (1 - (double) nSignFlips / nEvents) << std::endl;
  std::cout << "Electron Efficiency: " << (1 - (double) nElectronFlips / nEvents) << std::endl;
  std::cout << "Muon Efficiency: " << (1 - (double) nMuonFlips / nEvents) << std::endl;
}

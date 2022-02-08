#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/SignFlipModule.hh"

SignFlipModule::SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule):
  matchModule(imatchModule)
{
}

bool SignFlipModule::process()
{
  const auto matched = matchModule->getMatchingBestPairs().getPairs();

  for(const auto &particlePair : matched)
  {
	  nTotalEvents++;
	  
    auto particleType = particlePair.getGenParticle().getType();
    auto particlePt = particlePair.getGenParticle().getPt();

    if(particleType == Particle::Type::Electron) {
      if(particlePt > signFlipPtCut) {
        nHighPtElectronEvents++;
      }
      nElectronEvents++;
    } else if(particleType == Particle::Type::Muon) {
      if(particlePt > signFlipPtCut) {
        nHighPtMuonEvents++;
      }
      nMuonEvents++;
    }

	  if(particlePair.getGenParticle().charge() != particlePair.getRecoParticle().charge()) {
      if(particleType == Particle::Type::Electron) {
        nElectronFlips++;

        if(particlePt > signFlipPtCut) {
          nHighPtElectronFlips++;
        }
      } else if(particleType == Particle::Type::Muon) {
        nMuonFlips++;

        if(particlePt > signFlipPtCut) {
          nHighPtElectronFlips++;
        }
      }
		  nSignFlips++;
	  }
  }
  return true;
}
void SignFlipModule::finalize()
{
  std::cout << "Number of Events: " << nTotalEvents << std::endl;
  std::cout << "Number of Sign Flips: " << nSignFlips << std::endl;
  std::cout << "Electron Sign Efficiency: " << (1 - (double) nElectronFlips / nElectronEvents) << std::endl;
  std::cout << "Muon Sign Efficiency: " << (1 - (double) nMuonFlips / nMuonEvents) << std::endl;
  
  // Printing out rate of high pT sign flips and the number of high pT events
  std::cout << "High pT (> " << signFlipPtCut << "GeV) Electron Efficiency: " << (1 - (double) nHighPtElectronFlips / nHighPtElectronEvents) << " across " << nHighPtElectronEvents << " events." << std::endl;
  std::cout << "High pT (> " << signFlipPtCut << "GeV) Muon Efficiency: " << (1 - (double) nHighPtMuonFlips / nHighPtMuonEvents) << " across " << nHighPtMuonEvents << " events." << std::endl;
}

#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"


SignFlipModule::SignFlipModule(const std::shared_ptr<MatchingModule> imatchModule, int numMatchedPairs, int nTotalEvents, int nSignFlips, int nElectronFlips, int nElectronEvents, int nMuonFlips, int nMuonEvents):
  matchModule(imatchModule)
{
  addRequiredModule(imatchModule);
  this->numMatchedPairs = numMatchedPairs;
  this->nTotalEvents = nTotalEvents;
  this->nSignFlips = nSignFlips; 
  this-> nElectronFlips = nElectronFlips;
  this-> nElectronEvents = nElectronEvents;
  this-> nMuonFlips = nMuonFlips;
  this-> nMuonEvents = nMuonEvents;
}

bool SignFlipModule::process()
{
  const auto matched = matchModule->getMatchingBestPairs().getPairs();
  //std::cout << "Number of matched pairs: " << matched.size() << '\n';

 nTotalEvents++;

  //std::cout<<"the number of matched pairs is: " + numMatchedPairs;
  for (const auto &particlePair : matched)
  {
    numMatchedPairs++;

    auto particleType = particlePair.getRecoParticle().getType();
    auto particlePt = particlePair.getRecoParticle().getPt();
    
    if (particleType == ParticleType::electron()) 
    {
      
      if (particlePt > signFlipPtCut) 
      {
        nHighPtElectronEvents++;
      }
      nElectronEvents++;
      
    } 
    
    else if (particleType == ParticleType::muon())
    {
      if(particlePt > signFlipPtCut)
      {
        nHighPtMuonEvents++;

      }
      nMuonEvents++;

    
    }
    //std::cout << "Gen charge: " << particlePair.getGenParticle().getCharge() << "  Reco charge: " << particlePair.getRecoParticle().getCharge() << '\n';

	  if (particlePair.getGenParticle().getCharge() != particlePair.getRecoParticle().getCharge()) 
    {
      // static int positiveGenSimCounter = 0;
      // static int negativeGenSimCounter = 0;
      // static int positiveRecoCounter = 0;
      // static int negativeRecoCounter = 0;

      if (particleType == ParticleType::electron()) 
      {
        nElectronFlips++;
        if (particlePt > signFlipPtCut) 
        {
          nHighPtElectronFlips++;

        }
      } 
      else if (particleType == ParticleType::muon()) 
      {
        nMuonFlips++;
        if (particlePt > signFlipPtCut) 
        {
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
  std::cout << "Number of Matched Pairs: " << numMatchedPairs << std::endl;
  std::cout << "Number of Sign Flips: " << nSignFlips << std::endl;
  std::cout << "Electron Sign Efficiency: " << (1 - (double) nElectronFlips / nElectronEvents) << std::endl;
  std::cout << "Muon Sign Efficiency: " << (1 - (double) nMuonFlips / nMuonEvents) << std::endl;
  // Printing out rate of high pT sign flips and the number of high pT events
  std::cout << "High pT (> " << signFlipPtCut << "GeV) Electron Efficiency: " << (1 - (double) nHighPtElectronFlips / nHighPtElectronEvents) << " across " << nHighPtElectronEvents << " events." << std::endl;
  std::cout << "High pT (> " << signFlipPtCut << "GeV) Muon Efficiency: " << (1 - (double) nHighPtMuonFlips / nHighPtMuonEvents) << " across " << nHighPtMuonEvents << " events." << std::endl;
}
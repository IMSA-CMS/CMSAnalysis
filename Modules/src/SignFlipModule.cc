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
  
    auto particleType = particlePair.getGenParticle().getType();
    auto particlePt = particlePair.getGenParticle().getPt();
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;
    
    std::cout<<"lol";
    if (particleType == ParticleType::electron()) 
    {
      
      if (particlePt > signFlipPtCut) 
      {
        nHighPtElectronEvents++;
      }
      //std::cout << __FILE__ << " " << __LINE__ << std::endl;
      nElectronEvents++;

      //std::cout<<"hi";
      
    } 
    
    else if (particleType == ParticleType::muon())
    {
      if(particlePt > signFlipPtCut)
      {
        nHighPtMuonEvents++;
        //std::cout<<"french";
      }
      nMuonEvents++;
      //std::cout<<"spanish";
    
    }
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;
    //std::cout << "Gen charge: " << particlePair.getGenParticle().getCharge() << "  Reco charge: " << particlePair.getRecoParticle().getCharge() << '\n';

	  if (particlePair.getGenParticle().getCharge() != particlePair.getRecoParticle().getCharge()) 
    {
      if (particleType == ParticleType::electron()) 
      {
        nElectronFlips++;
        //std::cout<<"lol";
        //std::cout << __FILE__ << " " << __LINE__ << std::endl;
        if (particlePt > signFlipPtCut) 
        {
          nHighPtElectronFlips++;
          //std::cout<<"ahfiafpn";
        }
        //std::cout << __FILE__ << " " << __LINE__ << std::endl;
      } 
      else if (particleType == ParticleType::muon()) 
      {
        nMuonFlips++;
        //std::cout << __FILE__ << " " << __LINE__ << std::endl;
        if (particlePt > signFlipPtCut) 
        {
          nHighPtElectronFlips++;
        }
        //std::cout << __FILE__ << " " << __LINE__ << std::endl;
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
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"


MatchingModule::MatchingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, double deltaRCut) :
  genSim(genSimModule),
  reco(recoModule),
  deltaRCutoff(deltaRCut)
{
}

bool MatchingModule::process(const edm::EventBase& event)
{
  auto genSimParticles = genSim.getGenParticles();
  auto recoCandidates = reco.getRecoCandidates();


  //start with a high value, only really needs to be higher than the cutoff delta R
  double deltaRMin = 100;
  MatchedList pairDataList;
  //counters to set the indices to if the particle is best match
  int matchingBestPairsCounter = 0;
  int matchingCounter = 0;
  int matchingGenCounter = 0;
  //give a high default value so it won't pass the delta R limit unless it is reset with a real value
  double deltaR = 100;
  //keeps track of when the best particles are assigned 
  int bestGenIndex = 0;
  int bestIndex = 0;

  //loops through while there are still at least one gen and reco particle left that have not been matched and set to null
  while (!checkIsNull(genSimParticles) && !checkIsNull(recoCandidates))
    {
      //goes through all possible particle combinations of gen and reco particles
      for(auto& genParticle : genSimParticles)
	{
	  //checks that the particle was not already matched and set to null
	  if (genParticle!=NULL)
	    {
	      matchingCounter = 0;
	      for(auto& recoParticle : recoCandidates)
		{
		  if (recoParticle!=NULL)
		    {
		      double phiDif = findDeltaPhi(recoParticle->phi(), genParticle->phi()); 
		      deltaR = findDeltaR(recoParticle->eta(), genParticle->eta(), recoParticle->phi(), genParticle->phi(), phiDif); 
		      //if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
		      if (deltaR < deltaRMin)
			{
			  pairDataList.bestGenParticle = genParticle;
			  bestGenIndex = matchingGenCounter;
			  pairDataList.bestRecoParticle = recoParticle;
			  bestIndex = matchingCounter;
			  pairDataList.deltaR = deltaR;
			  deltaRMin = deltaR;
			  pairDataList.phiError=(phiDif);
			  pairDataList.etaError=calculateError(recoParticle->eta(), genParticle->eta());
			  pairDataList.ptError=calculateError(recoParticle->pt(), genParticle->pt());;
			}
		    }
		  ++matchingCounter;
		}

	    }
	  ++matchingGenCounter;
	}

      //makes an additional delta R cut and fills matching best pairs, resets values for the next loop
      //checks if the final (and best) delta R value for the matches passes the cut
      if(deltaRMin<deltaRCutoff)
	{
	  //keeps track of that match by adding it to the vector that will be returned
	  matchingBestPairs.push_back(pairDataList);

	  ++matchingBestPairsCounter;
	}
      //sets the best matches to null so they are not matched again
      genSimParticles[bestGenIndex] = nullptr;
      recoCandidates[bestIndex] = nullptr;
      matchingCounter = 0;
      matchingGenCounter = 0;
      //resets the values of delta R to prepare for another round of matching
      deltaRMin = 100;
      deltaR = 100;
    }

  return true;
}

double MatchingModule::findDeltaPhi(double recoPhi, double genPhi) const
{
  const double pi = 3.1415926535897932384;
  double actualPhiDif = recoPhi-genPhi;
  double phiDif = fabs(actualPhiDif);
  //  std::cout << "recoPhi " << recoPhi << " genPhi " << genPhi << " phiDif before " << phiDif;
  if(phiDif>pi)
    {
      phiDif = (2*pi)-phiDif;
    }
  //  std::cout << " phiDif after " << phiDif << '\n';
  return phiDif; 
}
 
double MatchingModule::findDeltaR(double recoEta, double genEta, double recoPhi, double genPhi, double phiDif) const
{  
  //  std::cout << "Phi dif: " << phiDif;
  double etaDif = recoEta-genEta; 
  double deltaR = sqrt((etaDif*etaDif)+(phiDif*phiDif));
  //  std::cout << " recoEta " << recoEta << " genEta " << genEta << " etaDif " << etaDif << " deltaR " << deltaR << '\n';
  return deltaR; 
}

double MatchingModule::calculateError(double exp, double theo) const
{
  return ((exp-theo)/theo); 
}

#include "MatchingModule.hh"


MatchingModule::MatchingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModuke& recoModule) :
  genSim(genSimModule),
  reco(recoModule)
{
}

std::vector<MatchingModule::MatchedList> MatchingModule::process(const edm::EventBase& event)
{
  //start with a high value, only really needs to be higher than the cutoff delta R
  double deltaRMin = 100;
  //vector that will be the return
  std::vector<matchedList> matchingBestPairs;
  matchedList pairDataList;
  //counters to set the indices to if the particle is best match
  int matchingBestPairsCounter = 0;
  int matchingCounter;
  int matchingGenCounter = 0;
  //give a high default value so it won't pass the delta R limit unless it is reset with a real value
  double deltaR = 100;
  //keeps track of when the best particles are assigned 
  int bestGenIndex = 0;
  int bestIndex = 0;

  //loops through while there are still at least one gen and reco particle left that have not been matched and set to null
  while (!checkIsNull(genSim) && !checkIsNull(reco))
    {

      //goes through all possible particle combinations of gen and reco particles
      for(auto& genParticle : genSim)
	{
	  //checks that the particle was not already matched and set to null
	  if (genParticle!=NULL)
	    {
	      matchingCounter = 0;
	      for(auto& recoParticle : reco)
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
      if(deltaRMin<0.1)
	{
	  //keeps track of that match by adding it to the vector that will be returned
	  matchingBestPairs.push_back(pairDataList);


	  ++matchingBestPairsCounter;
	  double genParticlePt = pairDataList.bestGenParticle->pt();
	  double indexDecimal = genParticlePt/50-1;
	  int index = (int)indexDecimal;
	  ++binPtCounter[index];
	  if (pairDataList.bestGenParticle->charge()!=pairDataList.bestRecoParticle->charge())
	    {
	      ++signFlipCounter[index];
	    }
	  
	}
      //sets the best matches to null so they are not matched again
      matchingGen[bestGenIndex] = nullptr;
      matching[bestIndex] = nullptr;
      matchingCounter = 0;
      matchingGenCounter = 0;
      //resets the values of delta R to prepare for another round of matching
      deltaRMin = 100;
      deltaR = 100;
    }

  return matchingBestPairs;
}
 

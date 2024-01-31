#include "CMSAnalysis/Modules/interface/MatchingModule.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include <limits>

MatchingModule::MatchingModule(double deltaRCut) :
  deltaRCutoff(deltaRCut)
{
}

bool MatchingModule::process() 
{
	matchingBestPairs.clear();

	// Make a copy so we don't modify the original
	std::vector<Particle> GenSimParticle(getInput()->getParticles(EventInput::RecoLevel::GenSim).getParticles());
	std::vector<Particle> recoCandidates(getInput()->getParticles(EventInput::RecoLevel::Reco).getParticles());
	return match(GenSimParticle, recoCandidates);
}
  
//loops through while there are still at least one gen and reco particle left that have not been matched and set to null
bool MatchingModule::match(std::vector<Particle> genSimParticles, std::vector<Particle> recoCandidates)
{

   matchingBestPairs.clear();
   unmatched.clear();
    while (!checkIsNull(genSimParticles) && !checkIsNull(recoCandidates))
    {
      //start with a high value, only really needs to be higher than the cutoff delta R
      double deltaRMin = std::numeric_limits<double>::max();

		Particle nullParticle(Particle::nullParticle());

		MatchingPair pairDataList(nullParticle, nullParticle);

		//counters to set the indices to if the particle is best match
		int matchingGenCounter = 0;
		int matchingRecoCounter = 0;

		//keeps track of when the best particles are assigned 
		int bestGenIndex = 0;
		int bestRecoIndex = 0;

		//goes through all possible particle combinations of gen and reco particles
      	for(auto& genParticle : genSimParticles)
		{
			auto genSimParticle = GenSimParticle(genParticle);
	  		//checks that the particle was not already matched and set to null
	  		if (genParticle.isNotNull() && genSimParticle == genSimParticle.finalDaughter())
	    	{
              	matchingRecoCounter = 0;  // Reset the Reco Counter
	      		for(auto& recoParticle : recoCandidates)
				{
		  			if (recoParticle.isNotNull())
		    		{
						// Casting is bad form, but we're just going to have to live with it.
						// Rewriting the code is too much work
						// Not checking the casts because they have to work
						// MatchingPair pairCandidate(dynamic_cast<const reco::GenParticle*>(genParticle),
						MatchingPair pairCandidate(genParticle, recoParticle);
						//if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
						if (pairCandidate.getDeltaR() < deltaRMin)
						{
							pairDataList = pairCandidate;
							bestGenIndex = matchingGenCounter;
							bestRecoIndex = matchingRecoCounter;
							deltaRMin = pairDataList.getDeltaR();
						}
		    		}
		  			++matchingRecoCounter;
				}
	    	}
	  		++matchingGenCounter;
		
		}
		// For each loop ends here

		//makes an additional delta R cut and fills matching best pairs, resets values for the next loop
		//checks if the final (and best) delta R value for the matches passes the cut
      	if(deltaRMin < deltaRCutoff)
		{
			//keeps track of that match by adding it to the vector that will be returned
			if(pairDataList.getGenParticle().isNotNull() && pairDataList.getRecoParticle().isNotNull())
			{
				matchingBestPairs.addMatchingPair(pairDataList);
			}
			
 		}
		//added to check deltaR cutoff

		//sets the best matches to null so they are not matched again
		
		genSimParticles.at(bestGenIndex) = nullParticle;
		recoCandidates.at(bestRecoIndex) = nullParticle;

		if (bestGenIndex == 0 && bestGenIndex == 0) //fixes error that causes matchmodule to get stuck on an event
		{
			break;
		}
		bestGenIndex = 0;
		bestRecoIndex = 0;
    }
	return true;
}

bool MatchingModule::checkIsNull(std::vector<Particle> matching) const
{
  	for (auto& particle : matching)
  	{
    	if(particle.isNotNull())
    	{
      	return false;
    	}
  	}	  
  	return true;
}

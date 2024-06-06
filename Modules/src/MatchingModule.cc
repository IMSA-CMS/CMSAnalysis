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
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;
	matchingBestPairs.clear();
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;

	//Make a copy so we don't modify the original
	std::vector<Particle> GenSimParticle(getInput()->getParticles(EventInput::RecoLevel::GenSim).getParticles());
	
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;
	
	std::vector<Particle> recoCandidates(getInput()->getParticles(EventInput::RecoLevel::Reco).getParticles());
	
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;
	
	bool ret = match(GenSimParticle, recoCandidates);
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;

	//std::cout<<"ret is: " + ret;
	return ret;
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
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;

		MatchingPair pairDataList(nullParticle, nullParticle);
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;

		//counters to set the indices to if the particle is best match
		int matchingGenCounter = 0;
		int matchingRecoCounter = 0;
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;

		//keeps track of when the best particles are assigned 
		int bestGenIndex = 0;
		int bestRecoIndex = 0;
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;

		//goes through all possible particle combinations of gen and reco particles
      	for(auto& genParticle : genSimParticles)
		{
			//std::cout << __FILE__ << " " << __LINE__ << std::endl;
			auto genSimParticle = GenSimParticle(genParticle);
			//std::cout << __FILE__ << " " << __LINE__ << std::endl;
	  		//checks that the particle was not already matched and set to null
	  		if (genParticle.isNotNull() && genSimParticle == genSimParticle.finalDaughter())
	    	{
				//std::cout << __FILE__ << " " << __LINE__ << std::endl;
              	matchingRecoCounter = 0;  // Reset the Reco Counter
	      		for(auto& recoParticle : recoCandidates)
				{
					//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		  			if (recoParticle.isNotNull())
		    		{
						// Casting is bad form, but we're just going to have to live with it.
						// Rewriting the code is too much work
						// Not checking the casts because they have to work
						// MatchingPair pairCandidate(dynamic_cast<const reco::GenParticle*>(genParticle),
						MatchingPair pairCandidate(genParticle, recoParticle);
						//std::cout << __FILE__ << " " << __LINE__ << std::endl;

						//if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
						if (pairCandidate.getDeltaR() < deltaRMin)
						{
							//std::cout << __FILE__ << " " << __LINE__ << std::endl;
							pairDataList = pairCandidate;
							bestGenIndex = matchingGenCounter;
							bestRecoIndex = matchingRecoCounter;
							deltaRMin = pairDataList.getDeltaR();
							//std::cout << __FILE__ << " " << __LINE__ << std::endl;
						}
						//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		    		}
		  			++matchingRecoCounter;
					//std::cout << __FILE__ << " " << __LINE__ << std::endl;
				}
	    	}
	  		++matchingGenCounter;
			//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		
		}
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		// For each loop ends here

		//makes an additional delta R cut and fills matching best pairs, resets values for the next loop
		//checks if the final (and best) delta R value for the matches passes the cut
      	if(deltaRMin < deltaRCutoff)
		{
			//keeps track of that match by adding it to the vector that will be returned
			if(pairDataList.getGenParticle().isNotNull() && pairDataList.getRecoParticle().isNotNull())
			{
				matchingBestPairs.addMatchingPair(pairDataList);
				//std::cout << __FILE__ << " " << __LINE__ << std::endl;
			}
			
 		}
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		//added to check deltaR cutoff

		//sets the best matches to null so they are not matched again
		
		genSimParticles.at(bestGenIndex) = nullParticle;
		recoCandidates.at(bestRecoIndex) = nullParticle;
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;

		if (bestGenIndex == 0 && bestGenIndex == 0) //fixes error that causes matchmodule to get stuck on an event
		{
			break;
			//std::cout << __FILE__ << " " << __LINE__ << std::endl;
		}
		bestGenIndex = 0;
		bestRecoIndex = 0;
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;
    }
	return true;
}

bool MatchingModule::checkIsNull(std::vector<Particle> matching) const
{
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;
  	for (auto& particle : matching)
  	{
    	if(particle.isNotNull())
    	{
			//std::cout << __FILE__ << " " << __LINE__ << std::endl;
      	return false;
    	}
		//std::cout << __FILE__ << " " << __LINE__ << std::endl;
  	}	  
	//std::cout << __FILE__ << " " << __LINE__ << std::endl;
  	return true;
}

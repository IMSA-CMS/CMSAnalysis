#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include <limits>

MatchingModule::MatchingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, double deltaRCut) :
  genSim(genSimModule),
  reco(recoModule),
  deltaRCutoff(deltaRCut)
{
}

bool MatchingModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING MatchingModule" << std::endl;
  matchingBestPairs.clear();

  // Make a copy so we don't modify the original
  std::vector<Particle> genSimParticles(genSim.getGenParticles().getParticles());
  std::vector<Particle> recoCandidates(reco.getRecoCandidates().getParticles());

  //loops through while there are still at least one gen and reco particle left that have not been matched and set to null
  //std::cerr << "Hey, I'm in charge of names here" << std::endl;
  while (!checkIsNull(genSimParticles) && !checkIsNull(recoCandidates))
    {
      //std::cerr << "Loop starts" << std::endl;
      //start with a high value, only really needs to be higher than the cutoff delta R
      double deltaRMin = std::numeric_limits<double>::max();

      Particle nullParticle(nullptr, Particle::LeptonType::None);

      MatchingPair pairDataList(nullParticle, nullParticle);

      //counters to set the indices to if the particle is best match
      int matchingGenCounter = 0;
      int matchingRecoCounter = 0;

      //keeps track of when the best particles are assigned 
      int bestGenIndex = 0;
      int bestRecoIndex = 0;

      //std::cerr << "I'm allergic to chocolate" << std::endl;

      //goes through all possible particle combinations of gen and reco particles
      for(auto& genParticle : genSimParticles)
	{
	  //checks that the particle was not already matched and set to null
	  if (genParticle.isNotNull())
	    {
              matchingRecoCounter = 0;  // Reset the Reco Counter
	      for(auto& recoParticle : recoCandidates)
		{
		  if (recoParticle.isNotNull())
		    {
		      // Casting is bad form, but we're just going to have to live with it.
		      // Rewriting the code is too much work
		      // Not checking the casts because they have to work
		      //MatchingPair pairCandidate(dynamic_cast<const reco::GenParticle*>(genParticle),
						 //dynamic_cast<const reco::RecoCandidate*>(recoParticle));
		      MatchingPair pairCandidate(genParticle, recoParticle);

			//std::cerr << "I'd like to do small things like rule the world" << std::endl;

		      //if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
		      if (pairCandidate.getDeltaR() < deltaRMin)
			{
			  pairDataList = pairCandidate;
			  //pairDataList.setGenParticle(genParticle);
			  //pairDataList.setRecoParticle(recoParticle);

			  bestGenIndex = matchingGenCounter;
			  bestRecoIndex = matchingRecoCounter;

			  deltaRMin = pairDataList.getDeltaR();
			}
		    }
		  ++matchingRecoCounter;
		}	
	    }
	  ++matchingGenCounter;
	  //if(!(pairDataList.getGenParticle().isNotNull()))
	  //{
	    //std::cerr << "NULL GEN PARTICLE" << std::endl;
	  //}
	  //else
	  //{
	    //std::cerr << "GOOD GEN" << std::endl;
	  //}
	  //if(!(pairDataList.getRecoParticle().isNotNull()))
	  //{
	    //std::cerr << "NULL RECO PARTICLE" << std::endl;
	  //}
	  //else
	  //{
	    //std::cerr << "GOOD RECO" << std::endl;
	  //}
	  //std::cerr << "It's tea, not leaf water" << std::endl;
		
	}

// For each loop ends here

      //makes an additional delta R cut and fills matching best pairs, resets values for the next loop
      //checks if the final (and best) delta R value for the matches passes the cut
      if(deltaRMin<deltaRCutoff)
	{
	  //keeps track of that match by adding it to the vector that will be returned
	  if(pairDataList.getGenParticle().isNotNull() && pairDataList.getRecoParticle().isNotNull())
	  {
	    matchingBestPairs.addMatchingPair(pairDataList);
	  }
	  
	}
      // else
      // 	{
      // 	  std::cout << "DeltaR Cutoff FAILED! Gen Sim CS = " + std::to_string(genSimCS) << std::endl;
      // 	}

      //sets the best matches to null so they are not matched again
	
      genSimParticles.at(bestGenIndex) = nullParticle;
      recoCandidates.at(bestRecoIndex) = nullParticle;

      //std::cerr << "Cabbages" << std::endl;

    }

  // if (!checkIsNull(genSimParticles) && checkIsNull(recoCandidates))
  //   {
  //     for (auto& particle : genSimParticles)
  // 	{
  // 	  if (particle)
  // 	    {
  // 	      std::cout << "Unmatched Particles Eta: " << particle->eta() << std::endl;
  // 	    }
  // 	}
  //   }
      
  //std::cerr << "EXITING MatchingModule" << std::endl;
  return true;
}

bool MatchingModule::checkIsNull(std::vector<Particle> matching) const
{
  for (auto& particle : matching)
  {
    if(particle.isNotNull())
    {
      //std::cerr << "Particle is not null" << std::endl;
      return false;
    }
  }  
  //std::cerr << "Particle is null" << std::endl;
  return true;
}

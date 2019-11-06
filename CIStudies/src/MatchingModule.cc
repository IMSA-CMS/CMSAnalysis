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
  matchingBestPairs.clear();

  // Make a copy so we don't modify the original
  std::vector<const reco::Candidate*> genSimParticles(genSim.getGenParticles().getParticles());
  std::vector<const reco::Candidate*> recoCandidates(reco.getRecoCandidates().getParticles());

  auto genSimCS = genSim.getGenParticles().getCollinsSoper();

  //loops through while there are still at least one gen and reco particle left that have not been matched and set to null
  while (!checkIsNull(genSimParticles) && !checkIsNull(recoCandidates))
    {
      //start with a high value, only really needs to be higher than the cutoff delta R
      double deltaRMin = std::numeric_limits<double>::max();

      MatchingPair pairDataList(nullptr, nullptr);

      //counters to set the indices to if the particle is best match
      int matchingCounter = 0;
      int matchingGenCounter = 0;

      //keeps track of when the best particles are assigned 
      int bestGenIndex = 0;
      int bestIndex = 0;

      //goes through all possible particle combinations of gen and reco particles
      for(auto& genParticle : genSimParticles)
	{
	  //checks that the particle was not already matched and set to null
	  if (genParticle)
	    {
	      for(auto& recoParticle : recoCandidates)
		{
		  if (recoParticle)
		    {
		      // Casting is bad form, but we're just going to have to live with it.
		      // Rewriting the code is too much work
		      // Not checking the casts because they have to work
		      MatchingPair pairCandidate(dynamic_cast<const reco::GenParticle*>(genParticle),
						 dynamic_cast<const reco::RecoCandidate*>(recoParticle));

		      //if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
		      if (pairCandidate.getDeltaR() < deltaRMin)
			{
			  pairDataList = pairCandidate;

			  bestGenIndex = matchingGenCounter;
			  bestIndex = matchingCounter;

			  deltaRMin = pairDataList.getDeltaR();
			}
		    }
		}

	    }
	  ++matchingGenCounter;
	}

      //makes an additional delta R cut and fills matching best pairs, resets values for the next loop
      //checks if the final (and best) delta R value for the matches passes the cut
      if(deltaRMin<deltaRCutoff)
	{
	  //keeps track of that match by adding it to the vector that will be returned
	  matchingBestPairs.addMatchingPair(pairDataList);
	}
      // else
      // 	{
      // 	  std::cout << "DeltaR Cutoff FAILED! Gen Sim CS = " + std::to_string(genSimCS) << std::endl;
      // 	}

      //sets the best matches to null so they are not matched again
      genSimParticles[bestGenIndex] = nullptr;
      recoCandidates[bestIndex] = nullptr;
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
      

  return true;
}

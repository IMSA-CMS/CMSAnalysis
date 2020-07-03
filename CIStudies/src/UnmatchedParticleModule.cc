#include "CIAnalysis/CIStudies/interface/UnmatchedParticleModule.hh" 

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

#include <iostream>
#include "TH2.h"

UnmatchedParticleModule::UnmatchedParticleModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const LRWeightModule& lrWeighting, const MatchingModule& matchingModule) :
  HistogramOutputModule(genSimModule, recoModule, weightingModule, lrWeighting),
  genSim(genSimModule),
  matching(matchingModule)
{
}

void UnmatchedParticleModule::initialize()
{  
  const int etaMin = -5;
  const int etaMax = 5;

  const int cosMin = -1;
  const int cosMax = 1;

  makeHistogram("UnmatchedParticleEta", "UnmatchedParticleEta", histBins, etaMin, etaMax);
  makeHistogram("MatchedParticleEta", "MatchedParticleEta", histBins, etaMin, etaMax);

  auto EtaCSHist = new TH2F("EtaCSHist", "EtaCSHist", histBins, cosMin, cosMax, histBins, etaMin, etaMax);
  addObject("EtaCSHist", EtaCSHist);

}

bool UnmatchedParticleModule::process(const edm::EventBase& event)
{
  auto genParticles = genSim.getGenParticles();
  auto genParticlesVector = genParticles.getParticles();


  auto matchingParticles = matching.getMatchingBestPairs().getGenParticles();
  auto matchingParticlesVector = matchingParticles.getParticles();


  if (matchingParticles.getNumParticles() < genParticles.getNumParticles())
    {
      for (auto& genParticle : genParticlesVector)
	{
	  bool isMatched = false;

	  auto collinsSoper = genParticles.getCollinsSoper();
	  auto eta = genParticle->eta();

	  auto EtaCSHist = dynamic_cast<TH2*>(getObject("EtaCSHist"));
	  EtaCSHist->Fill(collinsSoper, eta);

	  for (auto& matchingParticle : matchingParticlesVector)
	    {
	      if (genParticle == matchingParticle)
		{
		  isMatched = true;
		  break;
		}
	    }
	  
	  if (!isMatched)
	    {
	      fillHistogram("UnmatchedParticleEta", genParticle->eta());
	    }
	  else
	    {
	      fillHistogram("MatchedParticleEta", genParticle->eta());
	    }
	}
    }

      

  return true;
}

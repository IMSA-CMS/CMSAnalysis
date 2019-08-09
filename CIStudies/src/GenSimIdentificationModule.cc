#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"

bool GenSimIdentificationModule::process(const edm::EventBase& event)
{
  genParticles.clear();

  //Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
 
  const int electronCode = 11;
  const int muonCode = 13;

  auto particle = getFileParams().getParticle();

  int targetCode = particle == "Electron" ? electronCode : muonCode;

  //Begin GEN looping
  //Loop through Particle list&
  for (const auto& p : *genParticlesHandle)
    {
      if (particle == "Both" && (std::abs(p.pdgId()) == electronCode || std::abs(p.pdgId()) == muonCode))
	{
	  genParticles.addParticle(&p);
	}
      //Check for (anti)muon or (anti)electron
      else if (std::abs(p.pdgId()) == targetCode && isParticle(p))
	{ 
	  genParticles.addParticle(&p);
	  //std::cout << genParticles.size() << std::endl;
	}
    }
 return true;
}

bool GenSimIdentificationModule::isParticle(const reco::GenParticle& p) const
{
  const int finalStateParticleStatusCode = 1;
  const int maxQuarkCode = 6;
  const int maxLeptonCode = 13; // More than this is not a practical particle

  if (p.status() != finalStateParticleStatusCode || p.mother()->status() == finalStateParticleStatusCode || p.pt() < ptCut)
    return false;

  const reco::Candidate* nu = p.mother();
  int motherId = nu->pdgId();
  bool isParticle = true; 
  while (std::abs(motherId) > maxQuarkCode) // not a quark
    {
      if (nu->mother() && nu)
	{
	  nu = nu->mother(); 
	  motherId = nu->pdgId();

	  if (std::abs(motherId) > maxLeptonCode)
	    { // not a particle
	      isParticle = false; 
	    }
	  if (std::abs(motherId) <= maxQuarkCode)
	    {// is a quark 
	      isParticle = true;
	    }
	}		  
      else
	{
	  // This is pretty weird behavior, but clearly not a good particle if it gets here
	  return false;
	}
    }
  return isParticle;
}




#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"

GenSimIdentificationModule::GenSimIdentificationModule(int itargetPdgId):
  targetPdgId(itargetPdgId)
{
}

bool GenSimIdentificationModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING GenSimIdentificationModule" << std::endl;
  genParticles.clear();

  //Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
 
  const int electronCode = 11;
  const int muonCode = 13;

  auto particle = getFileParams().getLeptonType();

  int targetCode = particle == "Electron" ? electronCode : muonCode;

  //Begin GEN looping
  //Loop through Particle list&
  for (const auto& p : *genParticlesHandle)
    {
      if(p.status() == 1 && isParticle(Particle(&p, Particle::LeptonType::None)))
	{
	  if (particle == "Both" && (std::abs(p.pdgId()) == electronCode || std::abs(p.pdgId()) == muonCode))
	    {
	      if (std::abs(p.pdgId()) == electronCode)
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::Electron));
		}
	      else if (std::abs(p.pdgId()) == muonCode)
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::Muon));
		}
	      else
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::None));
		}
	    }
	  //Check for (anti)muon or (anti)electron
	  else if (std::abs(p.pdgId()) == targetCode)
	    { 
	      if (targetCode == electronCode)
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::Electron));
		}
	      else if (targetCode == muonCode)
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::Muon));
		}
	      else
		{
		  genParticles.addParticle(Particle(&p, Particle::LeptonType::None));
		}
	    }
	}
      //std::cout << "Number of particles: " << genParticles.getNumParticles() << std::endl;
    }
  //std::cerr << "EXITING GenSimIdentificationModule" << std::endl;
 return true;
}

bool GenSimIdentificationModule::isParticle(Particle p) const
{
  //std::cout << "Printing GenSim Particle: pdgID: " << p.pdgId() << ", status: " << p.status() << std::endl;

  const int finalStateParticleStatusCode = 1;
  const int maxQuarkCode = 6;
  const int maxLeptonCode = 13; // More than this is not a practical particle
  const int maxBosonCode = 24;

  if (p.status() != finalStateParticleStatusCode || p.mother().status() == finalStateParticleStatusCode)
    return false;

  if(targetPdgId != 0)
    {
      Particle currentMother = p;
      while(currentMother.mother().isNotNull())
	{
	  if(std::abs(currentMother.mother().pdgId()) == targetPdgId)
	    {
	      return true;
	    }
	  currentMother = currentMother.mother();
	}
      return false;
    }
  else
    {
      Particle nu = p.mother();
      int motherId = nu.pdgId();
      bool isParticle = true;

      //std::cout << "Printing GenSim Mother P: pdgID: " << nu.pdgId() << ", status: " << nu.status() << std::endl;

      while (std::abs(motherId) > maxQuarkCode) // not a quark
	{
	  if (nu.mother().isNotNull() && nu.isNotNull())
	    {
	      nu = nu.mother(); 
	      motherId = nu.pdgId();

	      if (std::abs(motherId) > maxLeptonCode && std::abs(motherId) <= maxBosonCode)
		{ // not a particle
		  //isParticle = false;
		  return false;
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
}




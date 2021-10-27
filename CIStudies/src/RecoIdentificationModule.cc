#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/Common/interface/Handle.h"

RecoIdentificationModule::RecoIdentificationModule(double iptCut) :
  ptCut(iptCut)
{}

bool RecoIdentificationModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING RecoIdentificationModule" << std::endl;
  recoCandidates.clear();

  auto particle = getFileParams().getLeptonType();

  if (particle == "Electron" || particle == "Both")
    {
      edm::Handle<std::vector<pat::Electron>> electrons;
      event.getByLabel(std::string("slimmedElectrons"), electrons);

      for (const auto& p : *electrons)
	{       
	  if (p.pt() > ptCut)
	    {
	      recoCandidates.addParticle(Particle(&p)); 
	    }
	}
    }

  if (particle == "Muon" || particle == "Both")
    {
      edm::Handle<std::vector<pat::Muon>> muons;
      event.getByLabel(std::string("slimmedMuons"), muons);

      for (const auto& p : *muons)
	{       
	  if (p.pt() > ptCut)
	    {
	      recoCandidates.addParticle(Particle(&p)); 
	    }
	  // else 
	  //   {
	  //     auto recoCS = recoCandidates.getCollinsSoper();
	  //     std::cout << "Pt Cut FAILED! Reco CS = " + std::to_string(recoCS) << " and Reco Pt = " + std::to_string(p.pt()) << std::endl;
	  //   }
	}
    }
  if (particle == "Photon" || particle == "Both")
    {
      edm::Handle<std::vector<pat::Photon>> photons;
      event.getByLabel(std::string("slimmedPhotons"), photons);
      std::cout << "Num of photons " << photons->size() << "\n";
      for (const auto& p : *photons)
	      {  
          std::cout << "Pt: " << p.pt() << "\n";     
	        if (p.pt() > ptCut)
	          {
	            recoCandidates.addParticle(Particle(&p)); 
	          }

	      }
    }
    
  
  //std::cerr << "number of leptons = " << recoCandidates.getNumParticles() << std::endl;

  //std::cerr << "EXITING RecoIdentificationModule" << std::endl;
  return true;
}

ParticleCollection RecoIdentificationModule::getRecoCandidates(Particle::Type leptonType) const
{
  ParticleCollection filteredCollection;

  for (const auto& particle : recoCandidates.getParticles())
  {
    if (particle.getType() == leptonType)
    {
      filteredCollection.addParticle(particle);
    }
  }

  return filteredCollection;
}

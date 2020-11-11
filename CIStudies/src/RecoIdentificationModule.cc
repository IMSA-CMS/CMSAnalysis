#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

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
	      recoCandidates.addParticle(Particle(&p, Particle::LeptonType::Electron)); 
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
	      recoCandidates.addParticle(Particle(&p, Particle::LeptonType::Muon)); 
	    }
	  // else 
	  //   {
	  //     auto recoCS = recoCandidates.getCollinsSoper();
	  //     std::cout << "Pt Cut FAILED! Reco CS = " + std::to_string(recoCS) << " and Reco Pt = " + std::to_string(p.pt()) << std::endl;
	  //   }
	}
    }

  //std::cerr << "EXITING RecoIdentificationModule" << std::endl;
  return true;
}

  

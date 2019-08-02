#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/Common/interface/Handle.h"

RecoIdentificationModule::RecoIdentificationModule(double iptCut) :
  ptCut(iptCut)
{}

bool RecoIdentificationModule::process(const edm::EventBase& event)
{
  recoCandidates.clear();

  if (getFileParams().getParticle() == "Electron")
    {
      edm::Handle<std::vector<pat::Electron>> electrons;
      event.getByLabel(std::string("slimmedElectrons"), electrons);

      for (const auto& p : *electrons)
	{       
	  if (p.pt() > ptCut)
	    {
	      recoCandidates.addParticle(&p); 
	    }
	}
    }

  else
    {
      edm::Handle<std::vector<pat::Muon>> muons;
      event.getByLabel(std::string("slimmedMuons"), muons);

      for (const auto& p : *muons)
	{       
	  if (p.pt() > ptCut)
	    {
	      recoCandidates.addParticle(&p); 
	    }
	}
    }
  
  return true;
}

  

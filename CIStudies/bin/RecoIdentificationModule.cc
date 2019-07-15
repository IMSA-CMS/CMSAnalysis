#include "RecoIdentificationModule.hh"
void RecoIdentificationModule::process(const edm::EventBase& event)
{

  std::vector<const reco::GenParticle*> recoCandidates;

  if (isElectronsIfTrue)
    {
      edm::Handle<std::vector<pat::Electron>> electrons;
      event.getByLabel(std::string("slimmedElectrons"), electrons);

      for (const auto& p : *electrons)
	{       
	  if (p.pt()>ptCut)
	    {
	      recoCandidates.push_back(&p); 
	    }
	}
    }

  else
    {
      edm::Handle<std::RecoIdentificvector<pat::Muon>> muons;
      event.getByLabel(std::string("slimmedMuons"), muons);

      for (const auto& p : *muons)
	{       
	  if (p.pt()>ptCut)
	    {
	      recoCandidates.push_back(&p); 
	    }
	}
    }
}

  

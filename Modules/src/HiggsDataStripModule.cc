#include "CMSAnalysis/Modules/interface/HiggsDataStripModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

#include "TFile.h"
#include "TTree.h"

#include <time.h>

void HiggsDataStripModule::initialize()
{
	tree = new TTree("Signal", "Lepton Jet Data");
	for (TTree *treeId : {tree})
	{
		treeId->Branch("pt", &pt1, "pt1/F");
		treeId->Branch("pt", &pt2, "pt2/F");
		treeId->Branch("phi", &phi1, "phi1/F");
		treeId->Branch("phi", &phi2, "phi2/F");
		treeId->Branch("eta", &eta1, "eta1/F");
		treeId->Branch("eta", &eta2, "eta2/F");
		treeId->Branch("deltaR", &deltaR, "deltaR/F");
		treeId->Branch("mass", &mass, "mass/F");

		std::cout << treeId->GetListOfBranches();
	}
}

HiggsDataStripModule::HiggsDataStripModule(const std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> iRecomod, std::shared_ptr<LeptonJetMatchingModule> iMatchmod) : recomod(iRecomod),
																																															matchmod(iMatchmod)
{
}

void HiggsDataStripModule::finalize()
{
	tree->Write();
}

bool HiggsDataStripModule::process()
{
	auto leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);
	double cutoff = 100;
	if (leptons.getNumParticles() < 2)
	{
		return true;
	}
	auto twoLeptons = leptons.chooseParticles(false);
	ParticleCollection<Particle> particles;
	particles.addParticle(twoLeptons.first);
	particles.addParticle(twoLeptons.second);
	if (twoLeptons.first.isNotNull() && twoLeptons.second.isNotNull())
	{
		if (particles.calculateSameSignInvariantMass(false) > cutoff)
		{
			if(particles.getLeptonTypeCount(ParticleType::muon()) == 2)
			{
			pt1 = twoLeptons.first.getPt();
			phi1 = twoLeptons.first.getPhi();
			eta1 = twoLeptons.first.getEta();
			pt2 = twoLeptons.second.getPt();
			phi2 = twoLeptons.second.getPhi();
			eta2 = twoLeptons.second.getEta();
			deltaR = twoLeptons.second.getDeltaR(twoLeptons.first);
			mass = particles.calculateSameSignInvariantMass(false);
			tree->Fill();
			}
		}
	}
	return true;
}
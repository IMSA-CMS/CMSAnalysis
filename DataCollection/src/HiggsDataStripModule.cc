#include "CMSAnalysis/DataCollection/interface/HiggsDataStripModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "TFile.h"
#include "TTree.h"

#include <time.h>

void HiggsDataStripModule::initialize()
{
	tree = new TTree("Signal", "Higgs Data");
	for (TTree *treeId : {tree})
	{
		treeId->Branch("pt", &pt1, "pt1/F");
		treeId->Branch("pt", &pt2, "pt2/F");
		treeId->Branch("phi", &phi1, "phi1/F");
		treeId->Branch("phi", &phi2, "phi2/F");
		treeId->Branch("eta", &eta1, "eta1/F");
		treeId->Branch("eta", &eta2, "eta2/F");
		treeId->Branch("deltaR", &deltaR, "deltaR/F");

		std::cout << treeId->GetListOfBranches();
	}
}

HiggsDataStripModule::HiggsDataStripModule(const std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> iRecomod, std::shared_ptr<LeptonJetMatchingModule> iMatchmod) : recomod(iRecomod),
																																															matchmod(iMatchmod)
{
}

void HiggsDataStripModule::writeAll()
{
	tree->Write(); // file->Close();
}

void HiggsDataStripModule::finalize()
{
}

bool HiggsDataStripModule::process()
{
	auto leptons = getInput()->getLeptons(InputModule::RecoLevel::Reco);
	double cutoff = 100;
	// Find source of null pointer error
	std::pair<Particle, Particle> twoLeptons = leptons.chooseParticles(false);
	ParticleCollection<Particle> particles;
	particles.addParticle(twoLeptons.first);
	particles.addParticle(twoLeptons.second);

	std::cout << "Line A0 ";
	if (particles.calculateSameSignInvariantMass(false) > cutoff)
	{

		std::cout << "Line A1"
				  << "\n";
		pt1 = twoLeptons.first.getPt();
		phi1 = twoLeptons.first.getPhi();
		eta1 = twoLeptons.first.getEta();
		pt2 = twoLeptons.second.getPt();
		phi2 = twoLeptons.second.getPhi();
		eta2 = twoLeptons.second.getEta();
		deltaR = twoLeptons.second.getDeltaR(twoLeptons.first);
		tree->Fill();
	}
	return true;
}
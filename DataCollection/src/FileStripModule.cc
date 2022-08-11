#include "CMSAnalysis/DataCollection/interface/FileStripModule.hh"
#include "TFile.h"

FileStripModule::FileStripModule(std::string name)
{
	file = new TFile(name.c_str(), "recreate");
	tree = new TTree("stripped", "Stripped Data");

	// Branches
	tree->Branch("elecSize", &elecSize);
	tree->Branch("elecEta", &elecEta);
	tree->Branch("elecPhi", &elecPhi);
	tree->Branch("elecMass", &elecMass);
	tree->Branch("elecCharge", &elecCharge);
	tree->Branch("elecPt", &elecPt);
	tree->Branch("elecReliso", &elecReliso);
	
	tree->Branch("muonSize", &muonSize);
	tree->Branch("muonEta", &muonEta);
	tree->Branch("muonPhi", &muonPhi);
	tree->Branch("muonMass", &muonMass);
	tree->Branch("muonCharge", &muonCharge);
	tree->Branch("muonPt", &muonPt);
	tree->Branch("muonReliso", &muonReliso);

	tree->Branch("metPhi", &metPhi);
	tree->Branch("metPt", &metPt);

	tree->Branch("jetSize", &jetSize);
	tree->Branch("jetEta", &jetEta);
	tree->Branch("jetPhi", &jetPhi);
	tree->Branch("jetMass", &jetMass);
	tree->Branch("jetPt", &jetPt);

	tree->Branch("genSize", &genSize);
	tree->Branch("genPID", &genPID);
	tree->Branch("genStatus", &genStatus);
	tree->Branch("genEta", &genEta);
	tree->Branch("genPhi", &genPhi);
	tree->Branch("genMass", &genMass);
	tree->Branch("genPt", &genPt);
	// tree->Branch("genD1", &genD1);
	// tree->Branch("genD2", &genD2);
	// tree->Branch("genD3", &genD3);
	// tree->Branch("genD4", &genD4);

	tree->Branch("elecIDPass", &elecIDPass);
	tree->Branch("muonIDPass", &muonIDPass);
}

bool FileStripModule::process()
{
	// Gets all of the electrons in the event
	auto electrons = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Electron);
	elecSize = electrons.getNumParticles();

	for (auto& electron : electrons)
	{
		elecEta.push_back(electron.getEta());
		elecPhi.push_back(electron.getPhi());
		elecMass.push_back(electron.getMass());
		elecCharge.push_back(electron.getCharge());
		elecPt.push_back(electron.getPt());
		auto l_electron = Lepton(electron);
		elecReliso.push_back(l_electron.getIsolation());
		int idPass = (l_electron.isTight() & 1) | (l_electron.isMedium() & 2) | (l_electron.isLoose() & 4);
		elecIDPass.push_back(idPass);
	}

	auto muons = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon);
	muonSize = muons.getNumParticles();

	for (auto& muon : muons)
	{
		muonEta.push_back(muon.getEta());
		muonPhi.push_back(muon.getPhi());
		muonMass.push_back(muon.getMass());
		muonCharge.push_back(muon.getCharge());
		muonPt.push_back(muon.getPt());
		auto l_muon = Lepton(muon);
		muonReliso.push_back(l_muon.getIsolation());
		int idPass = (l_muon.isTight() & 1) | (l_muon.isMedium() & 2) | (l_muon.isLoose() & 4);
		muonIDpass.push_back(idPass);
	}

	// Met

	auto jets = getInput()->getJets(InputModule::RecoLevel::Reco);
	jetSize = jets.getNumParticles();

	for (auto& jet : jets)
	{
		jetEta.push_back(jet.getEta());
		jetPhi.push_back(jet.getPhi());
		jetMass.push_back(jet.getMass());
		jetPt.push_back(jet.getPt());
	}

	auto genSim = getInput()->getParticles(InputModule::RecoLevel::Reco);
	genSize = genSim.getNumParticles();

	for (auto& particle : genSim)
	{
		auto genSimParticle = GenSimParticle(particle);
		genPID.push_back(genSimParticle.pdgId());
		genStatus.push_back(genSimParticle.status());
		genEta.push_back(genSimParticle.getEta());
		genPhi.push_back(genSimParticle.getPhi());
		genMass.push_back(genSimParticle.getMass());
		genPt.push_back(genSimParticle.getPt());
		std::vector<int> daughters = genSimParticle.getParticleImplementation()->daughters();
		genD1.push_back(daughters[0]);
		genD2.push_back(daughters[1]);
		std::vector<int> mothers = genSimParticle.getParticleImplementation()->mothers();
		genM1.push_back(mothers[0]);
		genM2.push_back(mothers[1]);
	}


	file->cd();
	tree->Fill();

	// Very important: clear the vectors
	elecEta.clear();
	elecPhi.clear();
	elecMass.clear();
	elecCharge.clear();
	elecPt.clear();
	elecReliso.clear();
	elecIDpass.clear();

	muonEta.clear();
	muonPhi.clear();
	muonMass.clear();
	muonCharge.clear();
	muonPt.clear();
	muonReliso.clear();
	muonIDpass.clear();

	jetEta.clear();
	jetPhi.clear();
	jetMass.clear();
	jetPt.clear();

	genPID.clear();
	genStatus.clear();
	genEta.clear();
	genPhi.clear();
	genMass.clear();
	genPt.clear();
	genD1.clear();
	genD2.clear();
	genD3.clear();
	genD4.clear();



	return true;
}

void FileStripModule::writeAll()
{
	file->Write();
	file->Close();
}
#include "CMSAnalysis/Modules/interface/FileStripModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "TFile.h"

FileStripModule::FileStripModule(std::string iname):
	name(iname)
{
}

void FileStripModule::initialize()
{
	file = new TFile(name.c_str(), "recreate");
	tree = new TTree("stripped", "Stripped Data");
	tree->SetAutoSave(0);

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
	tree->Branch("genD1", &genD1);
	tree->Branch("genD2", &genD2);
	tree->Branch("genM1", &genM1);
	tree->Branch("genM2", &genM2);

	tree->Branch("elecIDPass", &elecIDPass);
	tree->Branch("muonIDPass", &muonIDPass);
}

bool FileStripModule::process()
{

	auto electrons = getInput()->getParticles(InputModule::RecoLevel::Reco, ParticleType::electron());
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
	

	 auto muons = getInput()->getParticles(InputModule::RecoLevel::Reco, ParticleType::muon());
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
		 muonIDPass.push_back(idPass);
	}
	
	metPhi.push_back(-1.0);
	metPt.push_back(-1.0);

	jetSize = 0;
	genSize = 0;

	// Only work with particles in CandidateImplementation:

	// auto jets = getInput()->getJets(InputModule::RecoLevel::Reco);
	// jetSize = jets.getNumParticles();

	// for (auto& jet : jets)
	// {
	// 	jetEta.push_back(jet.getEta());
	// 	jetPhi.push_back(jet.getPhi());
	// 	jetMass.push_back(jet.getMass());
	// 	jetPt.push_back(jet.getPt());
	// }
	// auto genSim = getInput()->getParticles(InputModule::RecoLevel::GenSim);
	// genSize = genSim.getNumParticles();

	// for (auto& particle : genSim)
	// {
	// 	auto genSimParticle = GenSimParticle(particle);
	// 	genPID.push_back(genSimParticle.pdgId());
	// 	genStatus.push_back(genSimParticle.status());
	// 	genEta.push_back(genSimParticle.getEta());
	// 	genPhi.push_back(genSimParticle.getPhi());
	// 	genMass.push_back(genSimParticle.getMass());
	// 	genPt.push_back(genSimParticle.getPt());
	// 	auto genD1Particle = genSimParticle.daughter(1);
	// 	genD1.push_back(checkGenSim(genD1Particle, (genSim)));
	// 	auto genD2Particle = genSimParticle.daughter(2);
	// 	genD2.push_back(checkGenSim(genD2Particle, (genSim)));
	// 	auto genM1Particle = genSimParticle.mother();
	// 	genM1.push_back(checkGenSim(genM1Particle, (genSim))); 
	// 	genM2.push_back(-1);
	// } 

	file->cd();
	tree->Fill();

	// Very important: clear the vectors
	elecEta.clear();
	elecPhi.clear();
	elecMass.clear();
	elecCharge.clear();
	elecPt.clear();
	elecReliso.clear();
	elecIDPass.clear();

	muonEta.clear();
	muonPhi.clear();
	muonMass.clear();
	muonCharge.clear();
	muonPt.clear();
	muonReliso.clear();
	muonIDPass.clear();

	metPhi.clear();
	metPt.clear();

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
	genM1.clear();

	return true;
}

void FileStripModule::finalize()
{
	file->Write();
	file->Close();
}

int FileStripModule::checkGenSim(const GenSimParticle &genSim, ParticleCollection<Particle> &collection)
{
	// If the particle is not null, we need to find it in the GenSimParticles vector
	if (genSim.isNotNull())
	{
		int i = 0;
		for (auto particle : collection)
		{
			if (genSim == particle)
			{
				// std::cout << "Gen sim is not null\n";
				return i;
			}
			i++;
		}
		// std::cout << "gensim is not null, but not found in the list\n"
	}
	// std::cout << "gensim is null\n";
	return 0;
}

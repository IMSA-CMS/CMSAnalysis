#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"

RecoGenSimComparisonModule::RecoGenSimComparisonModule()
{

}

bool RecoGenSimComparisonModule::process()
{
	auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::None);
	auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim, Particle::Type::None);
	if (getInput()->getLeptons(InputModule::RecoLevel::Reco).calculateSameSignInvariantMass()>500){
		eventCounter++;
		printMatchInfo(recoParticles, genParticles);
	}
	return true;
}

void RecoGenSimComparisonModule::writeAll()
{

}

void RecoGenSimComparisonModule::finalize()
{
	std::cout << "\n\n\nRecoGenSimComparisonModule:\n\nPer reconstructed particle counters:\n";
	std::cout << "Total reconstructed particles: " << recoCounter << "\n";
	std::cout << "Reconstruction match ratio: " << (double)matchCounter/(double)recoCounter << "\n";
	std::cout << "Electron match ratio: " << (double)elecMatchCounter/(double)electronCounter << "\n";
	std::cout << "Muon match ratio: " << (double)muonMatchCounter/(double)muonCounter << "\n";
	std::cout << "High Eta match ratio " << (double)highEtaMatchCounter/(double)highEtaCounter << "\n";
	std::cout << "Accurate match ratio (same particle type): " << (double)accurateMatchCounter/(double)recoCounter << "\n";
	std::cout << "Accurate electron match ratio: " << (double)accElecMatchCounter/(double)electronCounter << "\n";
	std::cout << "Accurate muon match ratio: " << (double)accMuonMatchCounter/(double)muonCounter << "\n";
	std::cout << "Accurate high eta match ratio: " << (double)accHighEtaMatchCounter/(double)highEtaCounter << "\n";
	
	std::cout << "\nPer reconstructed event counters:\n";
	std::cout << "Total events: " << eventCounter << "\n";
	std::cout << "Total accurately measured events: " << accurateEventCounter << "\n";
	std::cout << "Total wrong charge events: " << wrongChargeCounter << "\n";
	std::cout << "Total mismeasured pT events: " << mismeasuredPtCounter << "\n";
	std::cout << "Total fake photon events: " << fakePhotonCounter << "\n";
	std::cout << "Total no match events: " << noMatchCounter << "\n";
}

void RecoGenSimComparisonModule::printMatchInfo(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";
	//event info
	output << "EVENT #" << eventCounter << "\n";
	output << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass() 
		<< " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass() << "\n";
	
	output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";

	int recoEventElement = 1;
	int genEventElement = 1;

	bool wrongCharge = false;
	bool fakePhoton = false;
	bool mismeasuredPt = false;
	bool accurate = false;
	bool noMatch = true;

	bool wrongChargeEvent = false;
	bool fakePhotonEvent = false;
	bool mismeasuredPtEvent = false;
	bool accurateEvent = false;
	bool noMatchEvent = true;
	
	//column labels
	output << std::left << std::setw(8) << "element" << std::setw(11) << "| type"
		<< std::setw(15) << "| charge"
		<< std::setw(15) << "| pT"
		<< std::setw(15) << "| Eta"
		<< std::setw(15) << "| Phi"
		<< std::setw(15) << "| E"
		<< std::setw(5) << "| mass" << std::endl;
	
	for(auto &recoPart : recoParts){
		// Standard info
		output << std::left << "++++++++++++++++ Reco Element ++++++++++++++++\n";
		output << std::setw(8) << recoEventElement << "| " << std::setw(9) << int(recoPart.getType()) << "| ";

		// Particle properties
		output << std::setw(13) << recoPart.getCharge() << "| " 
			<< std::setw(13) << recoPart.getPt() << "| " 
			<< std::setw(13) << recoPart.getEta() << "| " 
			<< std::setw(13) << recoPart.getPhi() << "| ";
		try
		{
			output << std::setw(13) << recoPart.getEnergy() << "| " 
			<< std::setw(13) << recoPart.getMass() << "\n";
		}
		catch(const std::exception& e)
		{
			output << std::setw(13) << "N/A" << "| " 
			<< std::setw(13) << recoPart.getMass() << std::endl;
		}

		recoEventElement++;
		recoCounter++;
		if(recoPart.getType()==Particle::Type::Electron)
		{
			electronCounter++;

		} else if (recoPart.getType()==Particle::Type::Muon)
		{
			muonCounter++;
		}
		if (recoPart.getEta() > 1.3)
		{
			highEtaCounter++;
		}

		wrongCharge = false;
		fakePhoton = false;
		mismeasuredPt = false;
		accurate = false;
		noMatch = true;

		output << std::left << "-------------- GenSim Matches --------------\n";

		//loop through all gensim particles
		for(auto &genPart : genParts){
			//match using deltaR (distance in phi-eta plane)
			double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
			if (deltaR < 0.1)
			{
				output << std::setw(8) << genEventElement << "| " << std::setw(9) << int(genPart.getType()) << "| ";
				// Particle properties
				output << std::setw(13) << genPart.getCharge() << "| " 
					<< std::setw(13) << genPart.getPt() << "| " 
					<< std::setw(13) << genPart.getEta() << "| " 
					<< std::setw(13) << genPart.getPhi() << "| ";
				try{
				output << std::setw(13) << genPart.getEnergy() << "| " 
					<< std::setw(13) << genPart.getMass() << std::endl;
				}
				catch(const std::exception& e){
				output << std::setw(13) << "N/A" << "| " 
					<< std::setw(13) << genPart.getMass() << std::endl;
				}

				matchCounter++;
				noMatch = false;
				
				if (recoPart.getType()==genPart.getType())
				{
					fakePhoton = false;
					if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge())
					{
						accurateMatchCounter++;
						accurate = true;
						break;
					}
					if(!((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1))
					{
						mismeasuredPt = true;
					}
					if (genPart.getCharge() != recoPart.getCharge())
					{
						wrongCharge = true;
					}
				}
				if (genPart.getType() == Particle::Type::Photon && !(mismeasuredPt || wrongCharge))
				{
					fakePhoton = true;
				}

				if(recoPart.getType()==Particle::Type::Electron)
				{
					elecMatchCounter++;
					if (recoPart.getType()==genPart.getType())
					{
						accElecMatchCounter++;
					}

				} else if (recoPart.getType()==Particle::Type::Muon)
				{
					muonMatchCounter++;
					if (recoPart.getType()==genPart.getType())
					{
						accMuonMatchCounter++;
					}
				}

				if (recoPart.getEta() > 1.3)
				{
					highEtaMatchCounter++;
					if (recoPart.getType()==genPart.getType())
					{
						accHighEtaMatchCounter++;
					}
				}
			}
			genEventElement++;
		}
		if (!accurate||noMatch)
		{
			accurateEvent = false;
			if (wrongCharge)
			{
				wrongChargeEvent = true;
			}
			if (mismeasuredPt)
			{
				mismeasuredPtEvent = true;
			}
			if (fakePhoton)
			{
				fakePhotonEvent = true;
			}
			if (noMatch)
			{
				noMatchEvent = true;
			}
		}
	}
	wrongChargeCounter += wrongChargeEvent;
	fakePhotonCounter += fakePhotonEvent;
	mismeasuredPtCounter += mismeasuredPtEvent;
	accurateEventCounter += accurateEvent;
	noMatchCounter += noMatchEvent;
}
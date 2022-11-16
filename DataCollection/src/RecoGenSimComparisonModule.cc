#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"

RecoGenSimComparisonModule::RecoGenSimComparisonModule()
{

}

bool RecoGenSimComparisonModule::process()
{
	auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco, ParticleType::none());
	auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none());
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
	// std::cout << "\n\n\nRecoGenSimComparisonModule:\n\nPer reconstructed particle counters:\n";
	// std::cout << "Total reconstructed particles: " << recoCounter << "\n";
	// std::cout << "Reconstruction match ratio: " << (double)matchCounter/(double)recoCounter << "\n";
	// std::cout << "Electron match ratio: " << (double)elecMatchCounter/(double)electronCounter << "\n";
	// std::cout << "Muon match ratio: " << (double)muonMatchCounter/(double)muonCounter << "\n";
	// std::cout << "High Eta match ratio " << (double)highEtaMatchCounter/(double)highEtaCounter << "\n";
	// std::cout << "Accurate match ratio (same particle type): " << (double)accurateMatchCounter/(double)recoCounter << "\n";
	// std::cout << "Accurate electron match ratio: " << (double)accElecMatchCounter/(double)electronCounter << "\n";
	// std::cout << "Accurate muon match ratio: " << (double)accMuonMatchCounter/(double)muonCounter << "\n";
	// std::cout << "Accurate high eta match ratio: " << (double)accHighEtaMatchCounter/(double)highEtaCounter << "\n";
	
	std::cout << "\nPer reconstructed event counters:\n";
	std::cout << "Total events: " << eventCounter << "\n";
	std::cout << "Total electron events: " << elecEventCounter << "\n";
	std::cout << "Total muon events: " << muonEventCounter << "\n";
	std::cout << "Accurately measured events ratio: " << accurateEventCounter/(double)eventCounter << "\n";
	std::cout << "Wrong charge events ratio: " << wrongChargeCounter/(double)eventCounter << "\n";
	std::cout << "Mismeasured pT events ratio: " << mismeasuredPtCounter/(double)eventCounter << "\n";
	std::cout << "Fake photon events ratio: " << fakePhotonCounter/(double)eventCounter << "\n";
	std::cout << "No match events ratio: " << noMatchCounter/(double)eventCounter << "\n";

	std::cout << "\nEssential to huge invariant mass:\n\n";
	std::cout << "Wrong charge electron events ratio: " << eEssWrongChargeCounter/(double)eventCounter << "\n";
	std::cout << "Mismeasured pT electron events ratio: " << eEssMismeasuredPtCounter/(double)eventCounter << "\n";
	std::cout << "Fake photon electron events ratio: " << eEssFakePhotonCounter/(double)eventCounter << "\n";
	std::cout << "No match electron events ratio: " << eEssNoMatchCounter/(double)eventCounter << "\n\n";

	std::cout << "Wrong charge muon events ratio: " << mEssWrongChargeCounter/(double)eventCounter << "\n";
	std::cout << "Mismeasured pT muon events ratio: " << mEssMismeasuredPtCounter/(double)eventCounter << "\n";
	std::cout << "Fake photon muon events ratio: " << mEssFakePhotonCounter/(double)eventCounter << "\n";
	std::cout << "No match muon events ratio: " << mEssNoMatchCounter/(double)eventCounter << "\n";
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

	bool accurateEvent = true;
	bool wrongChargeEvent = false;
	bool fakePhotonEvent = false;
	bool mismeasuredPtEvent = false;
	bool noMatchEvent = false;

	ParticleType recoPartType = ParticleType::photon();
	ParticleType wrongChargeType = ParticleType::photon();
	ParticleType fakePhotonType = ParticleType::photon();
	ParticleType mismeasuredPtType = ParticleType::photon();
	ParticleType noMatchType = ParticleType::photon();

	ParticleCollection<Particle> notWrongCharge;
	ParticleCollection<Particle> notFakePhoton;
	ParticleCollection<Particle> notMismeasuredPt;
	ParticleCollection<Particle> notNoMatch;
	
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
		output << "++++++++++++++++ Reco Element ++++++++++++++++\n";
		output << std::setw(8) << recoEventElement << "| " << std::setw(9) << recoPart.getName() << "| ";

		// // Particle properties
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
		if(recoPart.getType()==ParticleType::electron())
		{
			electronCounter++;

		} else if (recoPart.getType()==ParticleType::muon())
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
				output << std::setw(8) << genEventElement << "| " << std::setw(9) << genPart.getName() << "| ";
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

				if (genPart.getType() == ParticleType::photon() && !(mismeasuredPt || wrongCharge))
				{
					fakePhoton = true;
				}

				if(recoPart.getType()==ParticleType::electron())
				{
					elecMatchCounter++;
					if (recoPart.getType()==genPart.getType())
					{
						accElecMatchCounter++;
					}

				} else if (recoPart.getType()==ParticleType::muon())
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

		if (accurate)
		{
			notWrongCharge.addParticle(recoPart);
			notFakePhoton.addParticle(recoPart);
			notMismeasuredPt.addParticle(recoPart);
			notNoMatch.addParticle(recoPart);
		} else { accurateEvent = false; }

		if (!wrongCharge)
		{
			notWrongCharge.addParticle(recoPart);
		} else { 
			wrongChargeEvent = true; 
			if (wrongChargeType==ParticleType::photon())
			{
				wrongChargeType = recoPart.getType();
			}
		}

		if (!mismeasuredPt)
		{
			notMismeasuredPt.addParticle(recoPart);
		} else { 
			mismeasuredPtEvent = true; 
			if (mismeasuredPtType==ParticleType::photon())
			{
				mismeasuredPtType = recoPart.getType();
			}
		}

		if (!fakePhoton)
		{
			notFakePhoton.addParticle(recoPart);
		} else { 
			fakePhotonEvent = true; 
			if (fakePhotonType==ParticleType::photon())
			{
				fakePhotonType = recoPart.getType();
			}
		}

		if (!noMatch)
		{
			notNoMatch.addParticle(recoPart);
		} else { 
			noMatchEvent = true; 
			if (noMatchType==ParticleType::photon())
			{
				noMatchType = recoPart.getType();
			}
		}
		
	}

	if (notWrongCharge.calculateSameSignInvariantMass()<500){
		if (wrongChargeType == ParticleType::electron()) {
			eEssNoMatchCounter++;
		}
		if (wrongChargeType == ParticleType::muon()) {
			mEssWrongChargeCounter++;
		}
		recoPartType = wrongChargeType;
		output << "wrong charge essential " << notWrongCharge.calculateSameSignInvariantMass();
	}
	if (notFakePhoton.calculateSameSignInvariantMass()<500){
		if (recoPartType == ParticleType::electron()) {
			eEssNoMatchCounter++;
		}
		if (recoPartType == ParticleType::muon()) {
			mEssFakePhotonCounter++;
		}
		recoPartType = fakePhotonType;
		output << "| fake photon essential " << notFakePhoton.calculateSameSignInvariantMass();
	}
	if (notMismeasuredPt.calculateSameSignInvariantMass()<500){
		if (mismeasuredPtType == ParticleType::electron()) {
			eEssNoMatchCounter++;
		}
		if (mismeasuredPtType == ParticleType::muon()) {
			mEssMismeasuredPtCounter++;
		};
		recoPartType = mismeasuredPtType;
		output << "| mismeasured pt essential " << notMismeasuredPt.calculateSameSignInvariantMass();
	}
	if (notNoMatch.calculateSameSignInvariantMass()<500){
		if (noMatchType == ParticleType::electron()) {
			eEssNoMatchCounter++;
		}
		if (noMatchType == ParticleType::muon()) {
			mEssNoMatchCounter++;
		}
		recoPartType = noMatchType;
		output << "| no match essential " << notNoMatch.calculateSameSignInvariantMass();
	}
	output << "\n";
	if (recoPartType == ParticleType::electron()) {
		elecEventCounter++;
		std::cout << "electron event \n";
	}
	if (recoPartType == ParticleType::muon()) {
		muonEventCounter++;
		std::cout << "muon event \n";
	}

	wrongChargeCounter += wrongChargeEvent;
	fakePhotonCounter += fakePhotonEvent;
	noMatchCounter += noMatchEvent;
	mismeasuredPtCounter += mismeasuredPtEvent;
	accurateEventCounter += accurateEvent;
}
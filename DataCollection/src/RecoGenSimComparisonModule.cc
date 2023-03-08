#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"

RecoGenSimComparisonModule::RecoGenSimComparisonModule(std::string compType, bool writeOutput):
comparisonType(compType),
eventOutput(writeOutput)
{

}

bool RecoGenSimComparisonModule::process()
{
	auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco, ParticleType::none());
	auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none());
	
	printMatchInfo(recoParticles, genParticles);
	return true;
}

void RecoGenSimComparisonModule::writeAll()
{

}

void RecoGenSimComparisonModule::finalize()
{
	if (comparisonType == "SameSignInvariantMass")
	{
		std::cout << "\nPer reconstructed event counters:\n";
		std::cout << "Total events: " << eventCounter << "\n";
		std::cout << "Total electron events: " << elecEventCounter << "\n";
		std::cout << "Total muon events: " << muonEventCounter << "\n";
		std::cout << "Accurate event ratio: " << accurateEventCounter/(double)eventCounter << "\n";

		std::cout << "\nEssential to huge invariant mass:\n\n";
		std::cout << "Wrong charge electron events ratio: " << elecWrongChargeCounter/(double)eventCounter << "\n";
		std::cout << "Mismeasured pT electron events ratio: " << elecMismeasuredPtCounter/(double)eventCounter << "\n";
		std::cout << "Fake photon electron events ratio: " << elecFakePhotonCounter/(double)eventCounter << "\n";
		std::cout << "No match electron events ratio: " << elecNoMatchCounter/(double)eventCounter << "\n\n";
		std::cout << "Accurate electron event ratio: " << elecAccurateCounter/(double)eventCounter << "\n";

		std::cout << "\nWrong charge muon events ratio: " << muonWrongChargeCounter/(double)eventCounter << "\n";
		std::cout << "Mismeasured pT muon events ratio: " << muonMismeasuredPtCounter/(double)eventCounter << "\n";
		std::cout << "Fake photon muon events ratio: " << muonFakePhotonCounter/(double)eventCounter << "\n";
		std::cout << "No match muon events ratio: " << muonNoMatchCounter/(double)eventCounter << "\n";
		std::cout << "Accurate event ratio: " << muonAccurateCounter/(double)eventCounter << "\n";
	}
	if (comparisonType == "perParticle")
	{
		std::cout << "\nPer reconstructed event counters:\n";
		std::cout << "Total events: " << eventCounter << "\n";
		std::cout << "Total electrons: " << electronCounter << "\n";
		std::cout << "Total muons: " << muonCounter << "\n";

		std::cout << "\nEssential to huge invariant mass:\n\n";
		std::cout << "Wrong charge electrons ratio: " << elecWrongChargeCounter/(double)electronCounter << "\n";
		std::cout << "Mismeasured pT electrons ratio: " << elecMismeasuredPtCounter/(double)electronCounter << "\n";
		std::cout << "Fake photon electrons ratio: " << elecFakePhotonCounter/(double)electronCounter << "\n";
		std::cout << "No match electrons ratio: " << elecNoMatchCounter/(double)electronCounter << "\n\n";
		std::cout << "Accurate electron ratio: " << elecAccurateCounter/(double)electronCounter << "\n";

		std::cout << "\nWrong charge muons ratio: " << muonWrongChargeCounter/(double)muonCounter << "\n";
		std::cout << "Mismeasured pT muons ratio: " << muonMismeasuredPtCounter/(double)muonCounter << "\n";
		std::cout << "Fake photon muons ratio: " << muonFakePhotonCounter/(double)muonCounter << "\n";
		std::cout << "No match muons ratio: " << muonNoMatchCounter/(double)muonCounter << "\n";
		std::cout << "Accurate muon ratio: " << muonAccurateCounter/(double)muonCounter << "\n";
	}
	if (comparisonType == "fakePhoton")
	{
		std::cout << "\nPer reconstructed event counters:\n";
		std::cout << "Total events: " << eventCounter << "\n";
		std::cout << "Total electrons: " << electronCounter << "\n";
		std::cout << "Events with a photon/electron pair: " << numOfDesiredEvents << "\n";

		std::cout << "Fake photon electrons ratio: " << elecFakePhotonCounter/(double)electronCounter << "\n";

		std::cout << "Photon + electron events with high invariant mass: " << fakePhotonCounter/(double)eventCounter << "\n";
	}

	if (comparisonType == "mother") 
	{
		std::cout << "Total events: " << eventCounter << "\n";
		std::cout << "\nTotal 4-lepton events: " << numOfDesiredEvents << "\n";
		std::cout << "Total electron events: " << electronCounter << "\n";
		std::cout << "Total muon events: " << muonCounter << "\n";

		std::cout << "\nEvents with ISR" << isrCounter/(double)eventCounter << "\n";
		std::cout << "Events with FSR" << fsrCounter/(double)eventCounter << "\n";

		std::cout << "\nElectron events with ISR" << elecIsrCounter/(double)eventCounter << "\n";
		std::cout << "Electron events with FSR" << elecFsrCounter/(double)eventCounter << "\n";

		std::cout << "\nMuon events with ISR" << muonIsrCounter/(double)eventCounter << "\n";
		std::cout << "Muon events with FSR" << muonFsrCounter/(double)eventCounter << "\n";
	}
}

void RecoGenSimComparisonModule::printMatchInfo(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	eventCounter++;
	if (eventOutput)
	{
		output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";
		//event info
		output << "EVENT #" << eventCounter << "\n";
		output << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass() 
			<< " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass() << "\n";
		
		output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";
		//column labels
		output << std::left << std::setw(8) << "element" << std::setw(11) << "| type"
			<< std::setw(15) << "| charge"
			<< std::setw(15) << "| pT"
			<< std::setw(15) << "| Eta"
			<< std::setw(15) << "| Phi"
			<< std::setw(15) << "| E"
			<< std::setw(5) << "| mass" << std::endl;
	}
	
	if (comparisonType == "sameSignInvariantMass" && recoParts.calculateSameSignInvariantMass() > 500)
	{
		sameSignInvariantMassComparison(recoParts, genParts, output);
	}
	if (comparisonType == "perParticle")
	{
		perParticleComparison(recoParts, genParts, output);
	}
	if (comparisonType == "fakePhoton")
	{
		fakePhotonComparison(recoParts, genParts, output);
	}
	if (comparisonType == "mother")
	{
		mothersComparison(recoParts, genParts, output);
	}
}

void RecoGenSimComparisonModule::perParticleComparison(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	int recoEventElement = 1;
	int genEventElement = 1;

	bool wrongCharge = false;
	bool fakePhoton = false;
	bool mismeasuredPt = false;
	bool accurate = false;
	bool noMatch = true;
	auto recoType = ParticleType::photon();

	for(auto &recoPart : recoParts){
		// Standard info
		if (eventOutput) {
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
			output << std::left << "-------------- GenSim Matches --------------\n";
		}
		recoType = recoPart.getType();
		recoEventElement++;

		//loop through all gensim particles
		for(auto &genPart : genParts){
			//match using deltaR (distance in phi-eta plane)
			double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
			if (deltaR < 0.1)
			{
				if (eventOutput)
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
				}

				// matchCounter++;
				noMatch = false;
				
				if (recoPart.getType()==genPart.getType())
				{
					fakePhoton = false;
					if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge())
					{
						// accurateMatchCounter++;
						accurate = true;
						break;
					}
					

					if(!((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1))
					{
						mismeasuredPt = true;
					}

					if (genPart.getCharge() == - recoPart.getCharge())
					{
						wrongCharge = true;
					}
				}
				if (genPart.getType() == ParticleType::photon() && !(mismeasuredPt || wrongCharge))
				{
					fakePhoton = true;
				}
			}
			genEventElement++;
		}

		if (recoType==ParticleType::electron()) 
		{
			electronCounter++;
			elecWrongChargeCounter += wrongCharge;
			elecFakePhotonCounter += fakePhoton;
			elecNoMatchCounter += noMatch;
			elecMismeasuredPtCounter += mismeasuredPt;
			elecAccurateCounter += accurate;
		}
		else if (recoType==ParticleType::muon()) 
		{
			muonCounter++;
			muonWrongChargeCounter += wrongCharge;
			muonFakePhotonCounter += fakePhoton;
			muonNoMatchCounter += noMatch;
			muonMismeasuredPtCounter += mismeasuredPt;
			muonAccurateCounter += accurate;
		}
	}
}

void RecoGenSimComparisonModule::sameSignInvariantMassComparison(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	
	
	int recoEventElement = 1;
	int genEventElement = 1;

	bool wrongCharge = false;
	bool fakePhoton = false;
	bool mismeasuredPt = false;
	bool accurate = false;
	bool noMatch = true;

	bool accurateEvent = true;

	ParticleType recoPartType = ParticleType::photon();
	ParticleType wrongChargeType = ParticleType::photon();
	ParticleType fakePhotonType = ParticleType::photon();
	ParticleType mismeasuredPtType = ParticleType::photon();
	ParticleType noMatchType = ParticleType::photon();

	ParticleCollection<Particle> notWrongCharge;
	ParticleCollection<Particle> notFakePhoton;
	ParticleCollection<Particle> notMismeasuredPt;
	ParticleCollection<Particle> notNoMatch;
	
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

				// matchCounter++;
				noMatch = false;
				
				if (recoPart.getType()==genPart.getType())
				{
					fakePhoton = false;
					if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge())
					{
						// accurateMatchCounter++;
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
			}
			genEventElement++;
		}

		if (accurate)
		{
			notWrongCharge.addParticle(recoPart);
			notFakePhoton.addParticle(recoPart);
			notMismeasuredPt.addParticle(recoPart);
			notNoMatch.addParticle(recoPart);
		}

		if (!wrongCharge)
		{
			notWrongCharge.addParticle(recoPart);
		} else 
		{ 
			if (wrongChargeType==ParticleType::photon())
			{
				wrongChargeType = recoPart.getType();
			}
		}

		if (!mismeasuredPt)
		{
			notMismeasuredPt.addParticle(recoPart);
		} else { 
			// TODO: get the gensim particle: notMismeasuredPt.addParticle(Particle(genPart));
			if (mismeasuredPtType==ParticleType::photon())
			{
				mismeasuredPtType = recoPart.getType();
			}
		}

		if (!fakePhoton)
		{
			notFakePhoton.addParticle(recoPart);
		} else { 
			if (fakePhotonType==ParticleType::photon())
			{
				fakePhotonType = recoPart.getType();
			}
		}

		if (!noMatch)
		{
			notNoMatch.addParticle(recoPart);
		} else { 
			if (noMatchType==ParticleType::photon())
			{
				noMatchType = recoPart.getType();
			}
		}
		
	}

	if (notWrongCharge.calculateSameSignInvariantMass()<500)
	{
		accurateEvent = false;
		wrongChargeCounter++;
		recoPartType = wrongChargeType;
		if (wrongChargeType == ParticleType::electron()) 
		{
			elecWrongChargeCounter++;
			output << "wrong charge essential elec " << notWrongCharge.calculateSameSignInvariantMass() << std::endl;
		}
		if (wrongChargeType == ParticleType::muon()) 
		{
			muonWrongChargeCounter++;
			output << "wrong charge essential muon " << notWrongCharge.calculateSameSignInvariantMass() << std::endl;
		}
	}
	if (notFakePhoton.calculateSameSignInvariantMass()<500)
	{
		fakePhotonCounter++;
		accurateEvent = false;
		recoPartType = fakePhotonType;
		if (fakePhotonType == ParticleType::electron()) 
		{
			elecFakePhotonCounter++;
			output << "| fake photon essential elec " << notFakePhoton.calculateSameSignInvariantMass() << std::endl;
		}
		if (fakePhotonType == ParticleType::muon()) 
		{
			muonFakePhotonCounter++;
			output << "| fake photon essential muon " << notFakePhoton.calculateSameSignInvariantMass() << std::endl;
		}
	}
	if (notMismeasuredPt.calculateSameSignInvariantMass()<500)
	{
		mismeasuredPtCounter++;
		accurateEvent = false;
		recoPartType = mismeasuredPtType;
		if (mismeasuredPtType == ParticleType::electron()) 
		{
			elecMismeasuredPtCounter++;
			output << "| mismeasured pt essential elec " << notMismeasuredPt.calculateSameSignInvariantMass() << std::endl;
		}
		if (mismeasuredPtType == ParticleType::muon()) 
		{
			muonMismeasuredPtCounter++;
			output << "| mismeasured pt essential muon " << notMismeasuredPt.calculateSameSignInvariantMass() << std::endl;
		}
	}
	if (notNoMatch.calculateSameSignInvariantMass()<500)
	{
		noMatchCounter++;
		accurateEvent = false;
		recoPartType = noMatchType;
		if (noMatchType == ParticleType::electron()) 
		{
			elecNoMatchCounter++;
			output << "| no match essential elec " << notNoMatch.calculateSameSignInvariantMass() << std::endl;
		}
		if (noMatchType == ParticleType::muon()) 
		{
			muonNoMatchCounter++;
			output << "| no match essential muon " << notNoMatch.calculateSameSignInvariantMass() << std::endl;
		}
	}

	
	if (accurateEvent) 
	{
		accurateEventCounter++;
		recoPartType = notNoMatch.getParticles()[0].getType();
		if (notNoMatch.getParticles()[0].getType() == ParticleType::electron())
		{
			elecAccurateCounter++;
			output << "| accurate event electron " << notNoMatch.calculateSameSignInvariantMass() << std::endl;
		}
		if (notNoMatch.getParticles()[0].getType() == ParticleType::muon())
		{
			muonAccurateCounter++;
			output << "| accurate event muon " << notNoMatch.calculateSameSignInvariantMass() << std::endl;
		}
	}

	if (recoPartType == ParticleType::electron()) 
	{
		elecEventCounter++;
	}
	if (recoPartType == ParticleType::muon()) 
	{
		muonEventCounter++;
	}
}

void RecoGenSimComparisonModule::fakePhotonComparison(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	int recoEventElement = 1;
	int genEventElement = 1;

	bool fakePhoton = false;
	bool highInvMass = false;

	auto recoType = ParticleType::photon();

	for(auto &recoPart : recoParts)
	{
		// Standard info
		if (eventOutput) {
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
			output << std::left << "-------------- GenSim Matches --------------\n";
		}
		recoType = recoPart.getType();
		recoEventElement++;

		bool ePlusGamma = false;
		bool ePlusGammaEvent = false;

		for (auto &recoPart2 : recoParts)
		{
			ePlusGamma = (recoPart.getType() == ParticleType::electron() && recoPart2.getType() == ParticleType::photon() ) || (recoPart2.getType() == ParticleType::electron() && recoPart.getType() == ParticleType::photon());
			highInvMass = (recoPart.getFourVector() + recoPart2.getFourVector()).M() >  500;

			if (!ePlusGammaEvent) {
				ePlusGammaEvent = ePlusGamma;
			}
			if (ePlusGamma && highInvMass)
			{
				fakePhotonCounter++;
			}
		}
		numOfDesiredEvents+=ePlusGammaEvent;
	}
}

void RecoGenSimComparisonModule::mothersComparison(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output)
{
	int recoEventElement = 1;
	int genEventElement = 1;

	int leptonCount = 0;
	int elecCount = 0;
	int muonCount = 0;

	int fromQuark = 0;
	int fromElec = 0;

	bool elecEvent = false;
	bool muonEvent = false;

	bool isr = false;
	bool fsr = false;

	for(auto &recoPart : recoParts)
	{
		if (recoPart.getType() == ParticleType::electron() || recoPart.getType() == ParticleType::muon())
		{
			leptonCount++;
		}
	}

	if (leptonCount < 4)
	{
		return;
	}

	numOfDesiredEvents++;

	for(auto &recoPart : recoParts)
	{
		for (auto &genPart : genParts)
		{
			double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
			if (deltaR < 0.1)
			{
				if (recoPart.getType()==genPart.getType())
				{
					if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.25 && genPart.getCharge() == recoPart.getCharge())
					{
						if (!GenSimParticle(genPart).hasMother())
						{
							continue;
						}
						if (!GenSimParticle(genPart).mother().hasMother())
						{
							continue;
						}

						if (GenSimParticle(genPart).mother().mother().getType() == ParticleType::electron())
						{
							fromElec++;
						}
						else if (GenSimParticle(genPart).mother().mother().pdgId() < 7 && GenSimParticle(genPart).mother().mother().pdgId() > 0)
						{
							fromQuark++;
						}
						
					}					
				}
			}
			genEventElement++;
		}
		recoEventElement++;
	}

	if (elecCount > 2) 
	{
		electronCounter++;
	}
	if (muonCount > 2) 
	{
		muonCounter++;
	}

	if (fromQuark == 4) 
	{
		isr = true;
	}
	if (fromElec == 2)
	{
		fsr = true;
	}

	isrCounter += isr;
	fsrCounter += fsr;

	elecIsrCounter += isr && elecEvent;
	elecFsrCounter += fsr && elecEvent;
	muonIsrCounter += isr && muonEvent;
	muonFsrCounter += fsr && muonEvent;
}
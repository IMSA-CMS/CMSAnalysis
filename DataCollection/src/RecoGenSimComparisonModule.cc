#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"

RecoGenSimComparisonModule::RecoGenSimComparisonModule()
{

}

bool RecoGenSimComparisonModule::process()
{
	auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco, Particle::Type::None);
	auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim, Particle::Type::None);
	eventCounter++;
	if (getInput()->getLeptons(InputModule::RecoLevel::Reco).calculateSameSignInvariantMass()>500){
		printMatchInfo(recoParticles, genParticles);
	}
	return true;
}

void RecoGenSimComparisonModule::writeAll()
{

}

void RecoGenSimComparisonModule::printMatchInfo(const ParticleCollection<Particle>& recoParts, 
		const ParticleCollection<Particle>& genParts, 
		std::ostream& output) const
{
	output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << std::endl;
	//event info
	output << "EVENT #" << eventCounter << std::endl;
	output << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass() 
		<< " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass() << "\n";
	
	output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << std::endl;

	int recoEventElement = 1;
	int genEventElement = 1;

	//column labels
	output << std::left << std::setw(8) << "element" << std::setw(11) << "| type"
		<< std::setw(15) << "| charge"
		<< std::setw(15) << "| pT"
		<< std::setw(15) << "| Eta"
		<< std::setw(15) << "| Phi"
		<< std::setw(15) << "| E"
		<< std::setw(5) << "| mass\n";

	
	for(auto &recoPart : recoParts){
		// Standard info
		output << "++++++++++++++++ Reco Element ++++++++++++++++\n";
		output << std::setw(8) << recoEventElement << "| " << std::setw(9) << int(recoPart.getType()) << "| ";

		// Particle properties
		output << std::setw(13) << recoPart.getCharge() << "| " 
			<< std::setw(13) << recoPart.getPt() << "| " 
			<< std::setw(13) << recoPart.getEta() << "| " 
			<< std::setw(13) << recoPart.getPhi() << "| ";
		try{
		output << std::setw(13) << recoPart.getEnergy() << "| " 
			<< std::setw(13) << recoPart.getMass() << "\n";
		}
		catch(const std::exception& e){
		output << std::setw(13) << "N/A" << "| " 
			<< std::setw(13) << recoPart.getMass() << "\n";
		}
		recoEventElement++;

		output << "-------------- GenSim Matches --------------\n";

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
					<< std::setw(13) << genPart.getMass() << "\n";
				}
				catch(const std::exception& e){
				output << std::setw(13) << "N/A" << "| " 
					<< std::setw(13) << genPart.getMass() << "\n";
				}
			}
			genEventElement++;
		}
		
	}
}
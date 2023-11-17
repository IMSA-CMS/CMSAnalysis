#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"



LeptonJetMLStripModule::LeptonJetMLStripModule(std::shared_ptr<LeptonJetReconstructionModule> irecomod): 
recomod(irecomod)
{
    
}


void LeptonJetMLStripModule::addVariables()
{
    addVariable("leadingPt", SpecialVariableModule::VariableType::Float);
    addVariable("nParticles", SpecialVariableModule::VariableType::Integer);
	addVariable("eta", SpecialVariableModule::VariableType::Float);
	addVariable("phi", SpecialVariableModule::VariableType::Float);
	// reader.AddVariable("mass", &mass);
	addVariable("deltaR", SpecialVariableModule::VariableType::Float);
	addVariable("sumPt", SpecialVariableModule::VariableType::Float);
	addVariable("numMuons", SpecialVariableModule::VariableType::Integer);
	addVariable("deltaPt", SpecialVariableModule::VariableType::Float);

}

void LeptonJetMLStripModule::calculateVariables()
{

	auto leptonJets = recomod->getLeptonJets();            // inputs from LeptonJet

    // int jetIterator = 0;

    for (const auto &leptonJet : leptonJets)
    {
    	addValue("nParticles", leptonJet.getNumParticles());
		double sumPt = 0;
        // jetIndex = jetIterator;
		addValue("phi", leptonJet.getPhi());
		addValue("eta", leptonJet.getEta());
		double deltaR = 0;
		double leadingPt = 0;

        // mass = leptonJet.getMass();
       
		double runnerUpPt = 0;
		double numMuons = 0; //talk to dong about unadding after altering in 49, kind of weird that we don't have that
		//need to do this edit on 50;
        

        for (Particle p : leptonJet.getParticles())
        {
            for (Particle q : leptonJet.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
					deltaR = p.getDeltaR(q);
                }
            }
            if (p.getPt() > leadingPt)
            {
                runnerUpPt = leadingPt;
                leadingPt = p.getPt();
            }
            sumPt += p.getPt();
             
            if (p.getType() == ParticleType::muon())
            {
                numMuons++;
            }
        }
		addValue("deltaR", deltaR);
		addValue("deltaPt", leadingPt - runnerUpPt);
		addValue("leadingPt", leadingPt);
		addValue("sumPt", sumPt);
		addValue("numMuons", numMuons);


    }
}

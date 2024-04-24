#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"



LeptonJetMLStripModule::LeptonJetMLStripModule()
{
    
}


void LeptonJetMLStripModule::addVariables()
{
    //addVariable("leadingPt", SpecialVariableModule::VariableType::Float);
    addVariable("nParticles", SpecialVariableModule::VariableType::Float);
	
	//addVariable("phi", SpecialVariableModule::VariableType::Float);
	// reader.AddVariable("mass", &mass);
	
	addVariable("sumPt", SpecialVariableModule::VariableType::Float);
	//addVariable("numMuons", SpecialVariableModule::VariableType::Integer);
	addVariable("deltaPt", SpecialVariableModule::VariableType::Float);
	addVariable("deltaR", SpecialVariableModule::VariableType::Float);
	addVariable("eta", SpecialVariableModule::VariableType::Float);
	//addVariable("maxDXY", SpecialVariableModule::VariableType::Float);
	//addVariable("maxDZ", SpecialVariableModule::VariableType::Float);
	addVariable("maxIsolation", SpecialVariableModule::VariableType::Float);


}

void LeptonJetMLStripModule::calculateVariables(Particle particle)
{
	LeptonJet leptonJet(particle);//no need for loop
	//auto leptonJets = recomod->getLeptonJets();            // inputs from LeptonJet

    // int jetIterator = 0;

    //for (const auto &leptonJet : leptonJets)
    //{
        //std::cout << "Calling addValue for " << leptonJet.getNumParticles() <<'\n';
    	addValue("nParticles", static_cast<float>(leptonJet.getNumParticles()));
		double sumPt = 0;
        // jetIndex = jetIterator;
		addValue("phi", leptonJet.getPhi());
		addValue("eta", leptonJet.getEta());
		double deltaR = 0;
		double leadingPt = 0;
		double tempMaxDXY = 0;
		double tempMaxDZ = 0;
		double tempMaxIso = 0;

        // mass = leptonJet.getMass();
       
		double runnerUpPt = 0;
		double numMuons = 0; //talk to dong about unadding after altering in 49, kind of weird that we don't have that
		//need to do this edit on 50;
        

        for (Particle p : leptonJet.getParticles())
        {
			
			//p.getInfo("dxy");//max for dxy dz and islotion. simple max formula
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


//new stuff
			if (p.getInfo("dxy") > tempMaxDXY)
			{
				tempMaxDXY = p.getInfo("dxy");
			}


			if (p.getInfo("dz") > tempMaxDZ)
			{
				tempMaxDZ = p.getInfo("dz");
			}


			if (p.getInfo("Isolation") > tempMaxIso)
			{
				tempMaxIso = p.getInfo("Isolation");
			} 
            //if (p.getType() == ParticleType::muon())
            //{
            //    numMuons++;
            //}
        }
		addValue("deltaR", deltaR);
		addValue("deltaPt", leadingPt - runnerUpPt);
		//addValue("leadingPt", leadingPt);
		addValue("sumPt", sumPt);
		addValue("maxDXY", tempMaxDXY);
		addValue("maxDZ", tempMaxDZ);
		addValue("maxIsolation", tempMaxIso);
		//addValue("numMuons", numMuons);


    //}
}

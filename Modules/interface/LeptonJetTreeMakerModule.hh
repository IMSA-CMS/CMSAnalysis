#ifndef LEPTONJETTREEMAKERMODULE_HH
#define LEPTONJETTREEMAKERMODULE_HH

#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"

class LeptonJetTreeMakerModule : public TreeMakerModule
{
	public:
		using TreeMakerModule::TreeMakerModule;
		
		virtual std::string getName() override {return "LeptonJetTreeMakerModule";}
	
	protected:
		virtual std::vector<ParticleCollection<Particle>> getParticles() const override;

};

#endif
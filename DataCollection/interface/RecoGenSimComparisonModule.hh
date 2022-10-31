#ifndef RECOGENSIMCOMPARISONMODULE_HH
#define RECOGENSIMCOMPARISONMODULE_HH

#include <vector>
#include <cmath>

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

//compares event dumps of reco vs. gensim to match reconstructed particles to their corresponding 
class RecoGenSimComparisonModule : public AnalysisModule
{
    public:
        RecoGenSimComparisonModule();

		virtual bool process() override;
		virtual void writeAll() override;

	private:
	// prints information to the screen
	void printMatchInfo(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout) const;

	int numOfEvents = 0;
	int eventCounter = 0;
	int selectedcounter = 0;
};


#endif
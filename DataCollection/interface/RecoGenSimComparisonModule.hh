#ifndef RECOGENSIMCOMPARISONMODULE_HH
#define RECOGENSIMCOMPARISONMODULE_HH

#include <vector>
#include <cmath>

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"

//compares event dumps of reco vs. gensim to match reconstructed particles to their corresponding 
class RecoGenSimComparisonModule : public AnalysisModule
{
    public:
        RecoGenSimComparisonModule();

		virtual bool process() override;
		virtual void writeAll() override;
		virtual void finalize() override;

	private:
	// prints information to the screen
	void printMatchInfo(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);

	int numOfEvents = 0;
	int eventCounter = 0;
	//per particle counters
	int recoCounter = 0;
	int electronCounter = 0;
	int muonCounter = 0;
	int matchCounter = 0;
	int accurateMatchCounter = 0;
	int accElecMatchCounter = 0;
	int accMuonMatchCounter = 0;
	int accHighEtaMatchCounter = 0;
	int elecMatchCounter = 0;
	int muonMatchCounter = 0;
	int highEtaCounter = 0;
	int highEtaMatchCounter = 0;

	//per event counters
	int wrongChargeCounter = 0;
	int fakePhotonCounter = 0;
	int noMatchCounter = 0;
	int mismeasuredPtCounter = 0;
	int accurateEventCounter = 0;

	//
	int essWrongChargeCounter = 0;
	int essFakePhotonCounter = 0;
	int essNoMatchCounter = 0;
	int essMismeasuredPtCounter = 0;
};


#endif
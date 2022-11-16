#ifndef RECOGENSIMCOMPARISONMODULE_HH
#define RECOGENSIMCOMPARISONMODULE_HH

#include <vector>
#include <cmath>

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"

// compares event dumps of reco vs. gensim to match reconstructed particles to their 
// corresponding gensim particles
// currently specifically designed to find same sign invariant mass outliers in DY50 events
class RecoGenSimComparisonModule : public AnalysisModule
{
    public:
        RecoGenSimComparisonModule();

		virtual bool process() override;
		virtual void writeAll() override;
		
		//prints information to the screen
		virtual void finalize() override;

	private:
	// prints information to the screen
	void printMatchInfo(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);

	int numOfEvents = 0;
	int eventCounter = 0;
	int elecEventCounter = 0;
	int muonEventCounter = 0;
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

	// Per event counters - simply adds to the count if the event contains an occurence of respective mismeasurement
	// Doesn't consider the importance of the mismeasurement, compared to essential counters
	// An event can add to more than one counter at a time
	int wrongChargeCounter = 0;
	int fakePhotonCounter = 0;
	int noMatchCounter = 0;
	int mismeasuredPtCounter = 0;
	int accurateEventCounter = 0;

	// Essential event counters - only counts if the effect is a cause of the outlier mass
	// An event can add to more than one counter at a time
	// Prefixes: e = electron, m = muon
	int eEssWrongChargeCounter = 0;
	int eEssFakePhotonCounter = 0;
	int eEssNoMatchCounter = 0;
	int eEssMismeasuredPtCounter = 0;
	int mEssWrongChargeCounter = 0;
	int mEssFakePhotonCounter = 0;
	int mEssNoMatchCounter = 0;
	int mEssMismeasuredPtCounter = 0;
};


#endif
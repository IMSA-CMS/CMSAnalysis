#ifndef RECOGENSIMCOMPARISONMODULE_HH
#define RECOGENSIMCOMPARISONMODULE_HH
#include <vector>
#include <cmath>
#include <fstream>

#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"

// compares event dumps of reco vs. gensim to match reconstructed particles to their 
// corresponding gensim particles
// currently specifically designed to find same sign invariant mass outliers in DY50 events
class RecoGenSimComparisonModule : public AnalysisModule
{
    public:
        RecoGenSimComparisonModule(std::string compType = "perParticle", bool writeOutput = false);
        virtual bool process() override;
        //prints information to the screen
        virtual void finalize() override;

        virtual std::string getName() override {return "RecoGenSimComparisonModule";}
        
    private:
    // prints information to the screen
    void printMatchInfo(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);
    void printEventHeader(std::ostream& output_stream = std::cout);
    void printRecoPart(const Particle& recoPart, const int recoEventElement, std::ostream& output);
    void perParticleComparison(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);
    void sameSignInvariantMassComparison(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);
    void fakePhotonComparison(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);
    void mothersComparison(const ParticleCollection<Particle>& recoParticles, const ParticleCollection<Particle>& genParticles, std::ostream& output_stream = std::cout);
    std::string comparisonType;
    bool eventOutput;
    
    int eventCounter = 0;
    int numOfDesiredEvents = 0;
    int elecEventCounter = 0;
    int muonEventCounter = 0;
    int recoCounter = 0;
    int electronCounter = 0;
    int muonCounter = 0;
    // int highEtaMatchCounter = 0;
    // Essential event counters - only counts if the effect is a cause of the outlier mass
    // An event can add to more than one counter at a time
    int wrongChargeCounter = 0;
    int fakePhotonCounter = 0;
    int noMatchCounter = 0;
    int mismeasuredPtCounter = 0;
    int wrongTypeCounter = 0;
    int tauFakeCounter = 0;
    int accurateEventCounter = 0;
    // Essential event counters by particle type - only counts if the effect is a cause of the outlier mass
    // An event can add to more than one counter at a time
    // elec = electron
    int elecWrongChargeCounter = 0;
    int elecFakePhotonCounter = 0;
    int elecNoMatchCounter = 0;
    int elecMismeasuredPtCounter = 0;
    int elecWrongTypeCounter = 0;
    int elecFakeTauCounter = 0;
    int elecAccurateCounter = 0;
    int muonWrongChargeCounter = 0;
    int muonFakePhotonCounter = 0;
    int muonNoMatchCounter = 0;
    int muonMismeasuredPtCounter = 0;
    int muonWrongTypeCounter = 0;
    int muonFakeTauCounter = 0;
    int muonAccurateCounter = 0;
    // mothers vars
    int isrCounter = 0;
    int fsrCounter = 0;
    int neitherCounter = 0;
    int elecIsrCounter = 0;
    int elecFsrCounter = 0;
    int muonIsrCounter = 0;
    int muonFsrCounter = 0;

    int muon24Count = 0;
};

#endif

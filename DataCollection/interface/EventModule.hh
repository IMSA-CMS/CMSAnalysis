#ifndef EVENTMODULE_HH
#define EVENTMODULE_HH

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/LocalEventInputModule.hh"

// EventModule allows an Analyzer to select events and apply cuts.
// Additionally, contains a HistogramOutputModule to generate basic histograms for events.
class EventModule : public AnalysisModule
{
    public:
        EventModule();

        void addSelector(std::shared_ptr<Selector> selector);
        void addCut(std::shared_ptr<Cut> cut);
        
        // Prints the amount of events which passed each cut.
        void finalize() override;
        
        
        
        virtual void writeAll() override;

        const Event& getEvent() const {return event;}
        std::shared_ptr<HistogramOutputModule> getHistogramModule() {return histMod;}
    protected:
        bool process() override;

        // function to generate lambda functions for HistogramPrototype1DGeneral, so that they don't have to be explicitly declared
        std::function<std::vector<double>(const InputModule*)> findNthParticleFunction(int n, 
        Particle::Type particleType, InputModule::RecoLevel typeGenSim, double (Particle::* valueFunction)() const) const;

        // adds all Nth Highest Phi/Eta/Invariant Mass histograms for specified particleType up to the ith particle 
        // of that particle type
        void addBasicHistograms(Particle::Type particleType, int n);
        // returns "[n]th Highest [particle type] [value name]" e.g. 4th Highest Muon Eta
        std::string getBasicHistogramTitle(int n, Particle::Type particleType, std::string valueName) const;

    private:
        std::vector<std::shared_ptr<Selector>> selectors;
        std::vector<std::shared_ptr<Cut>> cuts;
        Event event;

        std::shared_ptr<HistogramOutputModule> histMod = std::make_shared<HistogramOutputModule>();
        LocalEventInputModule localInput;

        // Used for keeping track of the nth particles for which sets of histograms have been added
        std::unordered_set<std::string> basicHistograms;

        // Used for dynamically adding as many histograms as needed
        int maxElectrons = 0;
        int maxMuons = 0;
        int maxPhotons = 0;
        int maxJets = 0;
};

#endif
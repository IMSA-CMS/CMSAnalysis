#ifndef EVENTMODULE_HH
#define EVENTMODULE_HH

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>

#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"


// EventModule allows an Analyzer to select events and apply cuts.
// Additionally, contains a HistogramOutputModule to generate basic histograms for events.
// Add as an AnalysisModule in a Plan

class SingleParticleHist;
class CollectionHist;
class EventModule : public ProductionModule
{
    public:
        EventModule();

        void addSelector(std::shared_ptr<Selector> selector);
        void addCut(std::shared_ptr<Cut> cut);
        void addScaleFactor(std::shared_ptr<ScaleFactor> scaleFactors);
        virtual void setInput(const EventInput* input) override;
        void initialize() override;
        // Will print the amount of events which passed each cut.
        void finalize() override;

        const Event& getEvent() const {return event;}
        
        std::shared_ptr<HistogramOutputModule> getHistogramModule() {return histMod;}

        // Returns an input module that can be used to make these events the input to other modules
        const EventInput* getEventInput() const {return &localInput;}

        virtual std::string getName() override {return "EventModule";}

    protected:
        bool process() override;

        // function to generate lambda functions for HistogramPrototype1DGeneral, so that they don't have to be explicitly declared
        std::function<std::vector<double>(const EventInput*)> findNthParticleFunction(int n, 
        const ParticleType& particleType, EventInput::RecoLevel typeGenSim, double (Particle::* valueFunction)() const) const;

        // adds all Nth Highest Phi/Eta/Invariant Mass histograms for specified particleType up to the ith particle 
        // of that particle type
        void addBasicHistograms(const ParticleType& particleType, const ParticleCollection<Particle>& particles);

        // adds histograms which count the ammount of a certain type of particle
        void addCountHistograms(const ParticleType& particleType, const std::shared_ptr<ParticleCollection<Particle>> particles);

        // returns "[n]th Highest [particle type] [value name]" e.g. 4th Highest Muon Eta
        std::string getBasicHistogramTitle(int n, const ParticleType& particleType, std::string valueName) const;
        std::string getCountHistogramTitle(const ParticleType& particleType, std::string valueName) const;
        bool checkHist(std::string histName) const;

    private:
        void clearHistograms();
        std::vector<std::shared_ptr<Selector>> selectors;
        std::vector<std::shared_ptr<Cut>> cuts;
        std::vector<std::shared_ptr<ScaleFactor>> scaleFactors;
        Event event;

        std::shared_ptr<HistogramOutputModule> histMod = std::make_shared<HistogramOutputModule>();
        LocalEventInput localInput;

        // Used for keeping track of which sets of histograms have been added
        std::unordered_map<std::string,std::shared_ptr<SingleParticleHist>> particleHistograms;
        std::unordered_map<std::string,std::shared_ptr<CollectionHist>> collectionHistograms;


        // Used for dynamically adding as many histograms as needed
        int maxElectrons = 0;
        int maxMuons = 0;
        int maxPhotons = 0;
        int maxJets = 0;
};

#endif
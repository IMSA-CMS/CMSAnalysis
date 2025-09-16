#include "CMSAnalysis/Modules/interface/EventModule.hh"

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"

#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"

#include "CMSAnalysis/Histograms/interface/GetNthHighestPtHist.hh"
#include "CMSAnalysis/Modules/interface/SingleParticleHist.hh"
#include "CMSAnalysis/Modules/interface/CollectionHist.hh"
#include "CMSAnalysis/Histograms/interface/HistogramPrototype1DGeneral.hh"

static std::chrono::duration<double> time_1 = std::chrono::duration<double>::zero();
static std::chrono::duration<double> time_2 = std::chrono::duration<double>::zero();
static std::chrono::duration<double> time_3 = std::chrono::duration<double>::zero();


EventModule::EventModule():
    event(nullptr), localInput(&event)
{
    histMod->setInput(&localInput);
}

void EventModule::addSelector(std::shared_ptr<Selector> selector) 
{
    selectors.push_back(selector);

}
void EventModule::addScaleFactor(std::shared_ptr<ScaleFactor> scaleFactor)
{
    scaleFactors.push_back(scaleFactor);
}
void EventModule::addCut(std::shared_ptr<Cut> cut)
{
    cuts.push_back(cut);
}

void EventModule::finalize()
{
    for (int i = 0; i < int(cuts.size()); i++) {
        std::cout << "Dependent efficiency of " << typeid(*(cuts[i])).name() << ": " << cuts[i]->getDependentEfficiency() << "\n";
        std::cout << "Independent efficiency of " << typeid(*(cuts[i])).name() << ": " << cuts[i]->getIndependentEfficiency() << "\n";
        // std::cout << "total duration of first part: " << time_1.count() << std::endl;
        // std::cout << "total duration of second part: " << time_2.count() << std::endl;
        // std::cout << "total duration of third part: " << time_3.count() << std::endl;
    }
}

bool EventModule::process ()
{ 
    //auto start_1 = std::chrono::steady_clock::now();
  
    clearHistograms(); //all histograms are cleared and we only fill the ones we are using for this event
    event.clear();
    for (auto selector : selectors)
    {
        selector->selectParticles(getInput(),event);
    }

    event.setMET(getInput()->getMET());
    // auto end_1 = std::chrono::steady_clock::now();
    // time_1 += end_1 - start_1;


    // auto start_2 = std::chrono::steady_clock::now();
    bool passesCuts = true;

    //std::cout<<"\nthe cut size is: " <<cuts.size() << "\n";
    for (size_t i = 0; i < cuts.size(); i++) //The loop is not being entered because the cut size is 0 for our data set
    {
        if (!(cuts[i]->checkEvent(event, getInput(), passesCuts)))
        {
            passesCuts = false;
            break;
        }
    }

    if (!passesCuts)
    {
        return false;
    }

    // auto end_2 = std::chrono::steady_clock::now();
    // time_2 += end_2 - start_2;
    //auto start_3 = std::chrono::steady_clock::now();

    addBasicHistograms(ParticleType::electron(), event.getElectrons(), ParticleType::electron().getName());
    addBasicHistograms(ParticleType::muon(), event.getMuons(), ParticleType::muon().getName());
    addBasicHistograms(ParticleType::tau(), event.getTaus(), ParticleType::tau().getName());
    addBasicHistograms(ParticleType::photon(), event.getPhotons(), ParticleType::photon().getName());
    addBasicHistograms(ParticleType::jet(), event.getJets(),  ParticleType::jet().getName());

    //std::cout <<"The number of special events is: " << event.getSpecials().empty();

    //std::cout << "\nstart@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    
    try{
        for (auto& [key,value] : event.getSpecials())
        {
            auto specialPtr = std::make_shared<ParticleCollection<Particle>>(value);
            addBasicHistograms(value.getParticles()[0].getType(), value, key);
            addCountHistograms(value.getParticles()[0].getType(), specialPtr, key); 

        }
    }catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    auto electronCollection = std::make_shared<ParticleCollection<Particle>>(event.getElectrons());
    auto muonCollection = std::make_shared<ParticleCollection<Particle>>(event.getMuons());
    auto tauCollection = std::make_shared<ParticleCollection<Particle>>(event.getTaus());
    auto photonCollection = std::make_shared<ParticleCollection<Particle>>(event.getPhotons());
    auto jetCollection = std::make_shared<ParticleCollection<Particle>>(event.getJets());

    addCountHistograms(ParticleType::electron(), electronCollection, ParticleType::electron().getName());
    addCountHistograms(ParticleType::muon(), muonCollection, ParticleType::muon().getName());
    addCountHistograms(ParticleType::tau(), tauCollection, ParticleType::tau().getName());
    addCountHistograms(ParticleType::photon(), photonCollection, ParticleType::photon().getName());
    try{
        addCountHistograms(ParticleType::jet(), jetCollection, ParticleType::jet().getName());
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    //   std::cout << "EventModule event input: " << getInput() <<std::endl;
    //   int dummy;
    //   std::cin >> dummy;
    return true;
}

void EventModule::setInput(const EventInput* input)
{
    Module::setInput(input);
    event.setInput(input);
}

std::function<std::vector<double>(const EventInput*)> EventModule::findNthParticleFunction(int n, 
            const ParticleType& particleType, EventInput::RecoLevel typeGenSim, double (Particle::* valueFunction)() const) const
{
    std::function<std::vector<double>(const EventInput*)> NThParticleFunction = [n, particleType, typeGenSim, valueFunction] (const EventInput* input) -> std::vector<double> 
    {
        auto particles = input->getParticles(typeGenSim, particleType).getParticles();
        if (particles.size() > static_cast<size_t>(n))
        {
            return {((particles[n]).*(valueFunction))()};
        }
        return {};
    };
    return NThParticleFunction;
}


void EventModule::addBasicHistograms(const ParticleType& particleType, const ParticleCollection<Particle>& particles, std::string name)
{
    std::vector<Particle> parts = particles.getParticles();
    int count = 0;
    for (auto part : parts)
    {   

        for (auto params : particleType.getParticleHists())
        {
            auto histName = getBasicHistogramTitle(count, name, params.getName());
            if (!checkHist(histName))
            {
                params.setName(histName);
                auto histogram = std::make_shared<SingleParticleHist>(params);
                for (auto scaleFactor: scaleFactors)
                {
                    histogram->addScaleFactor(scaleFactor);
                }
                particleHistograms.insert({histName,histogram});
                histMod->addHistogram(histogram);

            }
            particleHistograms[histName]->setParticle(part);

        }
        count++;
    }
}

//CollectionHistparams ^^^
void EventModule::addCountHistograms(const ParticleType& particleType, const std::shared_ptr<ParticleCollection<Particle>> particles, std::string name)
{
    for (auto params : particleType.getCollectionHists())
    {
        auto histName = getCountHistogramTitle(name, params.getName());
        if (!checkHist(histName))
        {
            params.setName(histName);
            auto hist = std::make_shared<CollectionHist>(params);
            for (auto scaleFactor: scaleFactors)
            {
                hist->addScaleFactor(scaleFactor);
            }
            collectionHistograms.insert({histName, hist});
            histMod->addHistogram(hist);
        } 
        collectionHistograms[histName]->setCollection(particles);
    }
}

bool EventModule::checkHist(std::string histName) const
{
    bool inMap = false;
    auto it = particleHistograms.find(histName);
    if (it != particleHistograms.end())
    {
        inMap = true;
    }
    auto it2 = collectionHistograms.find(histName);
    if (it2 != collectionHistograms.end())
    {
        inMap = true;
    }
    return inMap;
}

std::string EventModule::getBasicHistogramTitle(int n, std::string particleType, std::string valueName) const
{
    n++;
    std::string rank = "th Highest ";
    if (n%10 == 1 && n%100 != 11) 
    {
        rank = "st Highest ";
    }
    if (n%10 == 2 && n%100 != 12)
    {
        rank = "nd Highest ";
    }
    if (n%10 == 3 && n%100 != 13)
    {
        rank = "rd Highest ";
    }
    return std::to_string(n) + rank + particleType + " " + valueName;
}

std::string EventModule::getCountHistogramTitle(std::string particleType, std::string valueName) const
{
    return particleType + " " + valueName;
}

void EventModule::clearHistograms()
{
    for (auto& [key,value] : particleHistograms)
    {
        value->clear();
    }
    for (auto& [key,value] : collectionHistograms)
    {
        value->clear();
    }
}

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

EventModule::EventModule():
    localInput(&event)
{
    histMod->setInput(&localInput);
}

void EventModule::addSelector(std::shared_ptr<Selector> selector) 
{
    selectors.push_back(selector);

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
    }
}

bool EventModule::process ()
{ 
    //std::cout<<"We have offically entered the method";
    clearHistograms(); //all histograms are cleared and we only fill the ones we are using for this event
    event.clear();

    for (auto selector : selectors)
    {
        selector->selectParticles(getInput(),event);
        //std::cout << __FILE__ << " " << __LINE__ << std::endl;
    }

    event.setMET(getInput()->getMET());
    bool passesCuts = true;
    //std::cout<<"right before for loop";

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

    addBasicHistograms(ParticleType::electron(), event.getElectrons());
    addBasicHistograms(ParticleType::muon(), event.getMuons());
    addBasicHistograms(ParticleType::photon(), event.getPhotons());
    addBasicHistograms(ParticleType::jet(), event.getJets());

    //std::cout <<"The number of special events is: " << event.getSpecials().empty();

    //std::cout << "\nstart@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    
    try{
        for (auto& [key,value] : event.getSpecials())
        {
            // CODE IS NOT GETTING HERE FOR SOME REASON, DEBUG THIS
            auto specialPtr = std::make_shared<ParticleCollection<Particle>>(value);
            addBasicHistograms(value.getParticles()[0].getType(), value);
            addCountHistograms(value.getParticles()[0].getType(), specialPtr); 

        }
    }catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    //std::cout << "end\n";

    auto electronCollection = std::make_shared<ParticleCollection<Particle>>(event.getElectrons());

    auto muonCollection = std::make_shared<ParticleCollection<Particle>>(event.getMuons());

    auto photonCollection = std::make_shared<ParticleCollection<Particle>>(event.getPhotons());
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;

    auto jetCollection = std::make_shared<ParticleCollection<Particle>>(event.getJets());
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;

    addCountHistograms(ParticleType::electron(), electronCollection);
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;
    
    addCountHistograms(ParticleType::muon(), muonCollection);
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;

    addCountHistograms(ParticleType::photon(), photonCollection);
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;

    try{addCountHistograms(ParticleType::jet(), jetCollection);
    //std::cout << __FILE__ << " " << __LINE__ << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    //std::cout <<"IT worked, maya and anwita did it!!";


    return true;
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


void EventModule::addBasicHistograms(const ParticleType& particleType, const ParticleCollection<Particle>& particles)
{
    std::vector<Particle> parts = particles.getParticles();
    int count = 0;
    for (auto part : parts)
    {   

        for (auto params : particleType.getParticleHists())
        {
            auto histName = getBasicHistogramTitle(count,particleType,params.getName());
            if (!checkHist(histName))
            {
                params.setName(histName);
                auto histogram = std::make_shared<SingleParticleHist>(params);
                particleHistograms.insert({histName,histogram});
                histMod->addHistogram(histogram);

            }
            particleHistograms[histName]->setParticle(part);

        }
        count++;
    }
}

//CollectionHistparams ^^^
void EventModule::addCountHistograms(const ParticleType& particleType, const std::shared_ptr<ParticleCollection<Particle>> particles)
{
    for (auto params : particleType.getCollectionHists())
    {
        auto histName = getCountHistogramTitle(particleType, params.getName());
        if (!checkHist(histName))
        {
            params.setName(histName);
            auto hist = std::make_shared<CollectionHist>(params);
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

std::string EventModule::getBasicHistogramTitle(int n, const ParticleType& particleType, std::string valueName) const
{
    n++;
    std::string rank = "th Highest ";
    if (n%10 == 1 && n%100 != 11) //I hate english words
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
    return std::to_string(n) + rank + particleType.getName() + " " + valueName;
}

std::string EventModule::getCountHistogramTitle(const ParticleType& particleType, std::string valueName) const
{
    return particleType.getName() + " " + valueName;
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
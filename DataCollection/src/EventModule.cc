#include "CMSAnalysis/DataCollection/interface/EventModule.hh"

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"

#include "CMSAnalysis/DataCollection/interface/GetNthHighestPtHist.hh"
#include "CMSAnalysis/DataCollection/interface/SingleParticleHist.hh"
#include "CMSAnalysis/DataCollection/interface/CollectionHist.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype1DGeneral.hh"



EventModule::EventModule():
localInput(&event)
{
    histMod->setInput(&localInput);
    //histMod->addHistogram(std::make_shared<HistogramPrototype1DGeneral>("MET", 150, 0, 1000, [] (const InputModule* input) -> std::vector<double> {return {input->getMET()};}));
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

void EventModule::writeAll() {}

bool EventModule::process ()
{
    clearHistograms(); //all histograms are cleared and we only fill the ones we are using for this event
    addCountHistograms(ParticleType::electron(), event.getElectrons());
    addCountHistograms(ParticleType::muon(), event.getMuons());
    addCountHistograms(ParticleType::photon(), event.getPhotons());
    addCountHistograms(ParticleType::jet(), event.getJets());
    for (auto& [key,value] : event.getSpecials())
    {
        addCountHistograms(value.getParticles()[0].getType(), value);
        //checks type of one particle, assuming all within a container have the same type
    }
    
    event.clear();

    for (auto selector : selectors) {
        selector->selectParticles(getInput(),event);
    }
    event.setMET(getInput()->getMET());
    bool passesCuts = true;
    if (event.containsParticles())
    {
        for (size_t i = 0; i < cuts.size(); i++) 
        {
            if (!(cuts[i]->checkEvent(event, passesCuts)))
            {
                passesCuts = false;
            }
        }
    }
    if (!passesCuts){
        return false;
    }
    addBasicHistograms(ParticleType::electron(), event.getElectrons());
    addBasicHistograms(ParticleType::muon(), event.getMuons());
    addBasicHistograms(ParticleType::photon(), event.getPhotons());
    addBasicHistograms(ParticleType::jet(), event.getJets());

    for (auto& [key,value] : event.getSpecials())
    {
        addBasicHistograms(value.getParticles()[0].getType(), value);
        //checks type of one particle, assuming all within a container have the same type
    }
    
    return true;
}

std::function<std::vector<double>(const InputModule*)> EventModule::findNthParticleFunction(int n, 
            const ParticleType& particleType, InputModule::RecoLevel typeGenSim, double (Particle::* valueFunction)() const) const
{
    std::function<std::vector<double>(const InputModule*)> NThParticleFunction = [n, particleType, typeGenSim, valueFunction] (const InputModule* input) -> std::vector<double> 
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
        for (auto hist : particleType.getHists())
        {
            auto histName = getBasicHistogramTitle(count,particleType,hist->getName());
            if (!checkHist(histName))
            {
                hist->changeName(histName);
                particleHistograms.insert({histName,hist});
                histMod->addHistogram(hist);
            }
            particleHistograms[histName]->setParticle(part);
        }
        count++;
    }
}

void EventModule::addCountHistograms(const ParticleType& particleType, const ParticleCollection<Particle>& particles)
{
//     std::string histName = "Number of " + particleType.getName() + "s";
//     if (!checkHist(histName))
//     {
//         auto hist = std::make_shared<CollectionHist>(histName, 11, -0.5, 10.5, [](const ParticleCollection<Particle>& collection){return std::vector<double>{collection.getNumParticles()};});
//         collectionHistograms.insert({histName,hist});
//         histMod->addHistogram(hist);
//     }
//     collectionHistograms[histName]->setCollection(particles);
}
bool EventModule::checkHist(std::string histName) const
{
    bool inMap = false;
    auto it = particleHistograms.find(histName);
    if (it != particleHistograms.end())
    {
        inMap = true;
    }
    // auto it2 = collectionHistograms.find(histName);
    //  if (it2 != collectionHistograms.end())
    // {
    //     inMap = true;
    // }
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

void EventModule::clearHistograms()
{
    for (auto& [key,value] : particleHistograms)
    {
        value->clear();
    }
    // for (auto& [key,value] : collectionHistograms)
    // {
    //     value->clear();
    // }
}
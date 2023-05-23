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
#include "CMSAnalysis/Utility/interface/SingleParticleHist.hh"
#include "CMSAnalysis/Histograms/interface/HistogramPrototype1DGeneral.hh"



EventModule::EventModule():
localInput(&event)
{
    histMod->setInput(&localInput);
    histMod->addHistogram(std::make_shared<HistogramPrototype1DGeneral>("MET", 150, 0, 1000, [] (const InputModule* input) -> std::vector<double> {return {input->getMET()};}));
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
    event.clear();
    // static int counter = 0;
    // std::cout << "Event# " <<  counter << "\n";
    // static int tightCount = 0;
    for (auto selector : selectors) {
        selector->selectParticles(getInput(),event);
    }
    event.setMET(getInput()->getMET());
    //std::cout << event.getMuons().getParticles().size() << "\n";
    bool passesCuts = true;
    if (event.containsParticles())
    {
        for (size_t i = 0; i < cuts.size(); i++) 
        {
            //std::cout << __LINE__ << "\n";
            if (!(cuts[i]->checkEvent(event, passesCuts)))
            {
                //std::cout << "EventModule didn't pass cuts \n";
                passesCuts = false;
            }
        }
    }
    if (!passesCuts){
        return false;
    }
    clearBasicHistograms(); //all histograms are cleared and we only fill the ones we are using for this event
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
                basicHistograms.insert({histName,hist});
                histMod->addHistogram(hist);
            }
            basicHistograms[histName]->setParticle(part);
        }
        count++;
    }
}

bool EventModule::checkHist(std::string histName) const
{
    bool inMap = true;
    auto it = basicHistograms.find(histName);
    if (it == basicHistograms.end())
    {
        inMap = false;
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

void EventModule::clearBasicHistograms()
{
    for (auto& [key,value] : basicHistograms)
    {
        value->clear();
    }
}
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
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype1DGeneral.hh"


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
        std::cout << "Dependent efficiency of " << typeid(*(cuts[i])).name() << ": " << cuts[i]->getDependentEfficiency() << "\n";\
        std::cout << "Independent efficiency of " << typeid(*(cuts[i])).name() << ": " << cuts[i]->getIndependentEfficiency() << "\n";\
    }
}

void EventModule::writeAll() {}

bool EventModule::process ()
{
    event.clear();
    int electronCount = 0;
    int muonCount = 0;
    int photonCount = 0;
    int jetCount = 0;
    // static int counter = 0;
    // std::cout << "Event# " <<  counter << "\n";
    for (auto selector : selectors) {
        selector->selectParticles(getInput(),event);
        // for (auto particle : selector->selectParticles(getInput()))
        // {
        //     switch (particle.getType()) 
        //     {
        //         case Particle::Type::Electron:
        //             event.addElectron(particle);
        //             electronCount++;
        //             break;
        //         case Particle::Type::Muon:
        //             event.addMuon(particle);
        //             muonCount++;
        //             break;
        //         case Particle::Type::Photon:
        //             event.addPhoton(particle);
        //             photonCount++;
        //             break;
        //         case Particle::Type::Jet:
        //             event.addJet(particle);
        //             jetCount++;
        //             break;
        //         default:
        //             throw std::runtime_error("this particle type cannot be added to event");
        //     }
        // }
    }
    event.setMET(getInput()->getMET());
    event.sort();
    
    //std::cout << event.getMuons().getParticles().size() << "\n";
    bool passesCuts = true;
    if (event.containsParticles())
    {
        for (int i = 0; i < int(cuts.size()); i++) 
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
    /*if (maxElectrons < electronCount || maxMuons < muonCount || maxPhotons < photonCount || maxJets < jetCount) 
    {
        maxElectrons = std::max(electronCount, maxElectrons);
        addBasicHistograms(Particle::Type::Electron);
        maxMuons = std::max(muonCount, maxMuons);
        addBasicHistograms(Particle::Type::Muon);
        maxPhotons = std::max(photonCount, maxPhotons);
        addBasicHistograms(Particle::Type::Photon);
        maxJets = std::max(jetCount, maxJets);
        addBasicHistograms(Particle::Type::Jet);
    }*/
    addBasicHistograms(Particle::Type::Electron, event.getElectrons().getNumParticles());
    addBasicHistograms(Particle::Type::Muon, event.getMuons().getNumParticles());
    addBasicHistograms(Particle::Type::Photon, event.getPhotons().getNumParticles());
    addBasicHistograms(Particle::Type::Jet, event.getJets().getNumParticles());
    for (auto& [key,value] : event.getSpecials())
    {
        addBasicHistograms(Particle::identifyType(key), value.getNumParticles());
    }
    return true;
}

std::function<std::vector<double>(const InputModule*)> EventModule::findNthParticleFunction(int n, 
            Particle::Type particleType, InputModule::RecoLevel typeGenSim, double (Particle::* valueFunction)() const) const
{
    std::function<std::vector<double>(const InputModule*)> a = [n, particleType, typeGenSim, valueFunction] (const InputModule* input) -> std::vector<double> 
    {
        auto particles = input->getParticles(typeGenSim, particleType);
        int count = 0;
        //std::cout << n << "\n";
        std::sort(particles.begin(), particles.end(), [](auto p1, auto p2) {return p1.getPt()>p2.getPt();});
        for (auto particle : particles.getParticles())
        {
            if (particle.getType() == particleType)
            {
                count++;
                if (count == n)
                {
                    // std::cout << "returns non-empty vector \n";
                    return {((particle).*(valueFunction))()};
                }
            }
        }
        //std::cout << "empty vector \n";
        return {};
    };
    return a;
}

void EventModule::addBasicHistograms(Particle::Type particleType, int n)
{

    int maxCount = n;
    /*
    switch (particleType)
    {
        case Particle::Type::Electron:
            maxCount = maxElectrons;
            break;
        case Particle::Type::Muon:
            maxCount = maxMuons;
            break;
        case Particle::Type::Photon:
            maxCount = maxPhotons;
            break;
        case Particle::Type::Jet:
            maxCount = maxJets;
            break;
        default:
            throw std::runtime_error("this particle type cannot be added to event");
    }
    */
    for (int i = 1; i <= maxCount; i++)
    {
        if (basicHistograms.find(getBasicHistogramTitle(i, particleType, "")) == basicHistograms.end()) 
        {
            histMod->addHistogram(std::make_shared<HistogramPrototype1DGeneral>(getBasicHistogramTitle(i, particleType, "Pt"), 150, 0, 1000, findNthParticleFunction(i, particleType, InputModule::RecoLevel::GenSim, &Particle::getPt)));
            histMod->addHistogram(std::make_shared<HistogramPrototype1DGeneral>(getBasicHistogramTitle(i, particleType, "Phi"), 150, -4, 4, findNthParticleFunction(i, particleType, InputModule::RecoLevel::GenSim, &Particle::getPhi)));
            histMod->addHistogram(std::make_shared<HistogramPrototype1DGeneral>(getBasicHistogramTitle(i, particleType, "Eta"), 150, -10, 10, findNthParticleFunction(i, particleType, InputModule::RecoLevel::GenSim, &Particle::getEta)));
            basicHistograms.emplace(getBasicHistogramTitle(i, particleType, "")); //maybe i should make basicHistograms store all the hists instead of only the boilerplate titles
        }
    }
}

std::string EventModule::getBasicHistogramTitle(int n, Particle::Type particleType, std::string valueName) const
{
    std::unordered_map<Particle::Type, std::string> particleTypeToName;
    particleTypeToName[Particle::Type::Electron] = "Electron";
    particleTypeToName[Particle::Type::Muon] = "Muon";
    particleTypeToName[Particle::Type::Photon] = "Photon";
    particleTypeToName[Particle::Type::Jet] = "Jet";
    particleTypeToName[Particle::Type::DarkPhoton] = "DarkPhoton";
    particleTypeToName[Particle::Type::Neutralino] = "Neutralino";
    particleTypeToName[Particle::Type::LeftHiggs] = "Left Higgs";
    particleTypeToName[Particle::Type::RightHiggs] = "Right Higgs";
    particleTypeToName[Particle::Type::ZBoson] = "Z Boson";
    std::string rank = "th Highest ";
    if (n%10 == 1)
    {
        rank = "st Highest ";
    }
    if (n%10 == 2)
    {
        rank = "nd Highest ";
    }
    if (n%10 == 3)
    {
        rank = "rd Highest ";
    }
    return std::to_string(n) + rank + particleTypeToName[particleType] + " " + valueName;
}
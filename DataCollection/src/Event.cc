#include "CMSAnalysis/DataCollection/interface/Event.hh"

#include <vector>
#include <unordered_map>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Cut.hh"


Event::Event()
{

}

ParticleCollection<Particle> Event::getParticles() const
{
    ParticleCollection<Particle> particleList;
    /*
    std::vector<ParticleCollection<Particle>*> particleCollections = {&electrons, &muons, &photons, &jets};
    for (const auto& collection : particleCollections)
    {
        for (const auto& p: collection->getParticles())
        {
            particleList.addParticle(Particle(p));
        }
    }
    */
    for (const auto& p: muons.getParticles())
    {
        particleList.addParticle(Particle(p));
    }
    for (const auto& p: electrons.getParticles())
    {
        particleList.addParticle(Particle(p));
    }
    for (const auto& p: photons.getParticles())
    {
        particleList.addParticle(Particle(p));
    }
    for (const auto& p: jets.getParticles())
    {
        particleList.addParticle(Particle(p));
    }
    for (auto& [key,value] : specialObjects)
    {
        for (const auto& p: value.getParticles())
        {
           particleList.addParticle(Particle(p)); 
        }
    }
    return particleList;
}

void Event::addElectron(Electron electron) 
{
    electrons.addParticle(electron); 
    electrons.sort();
}

void Event::addMuon(Muon muon) 
{
    muons.addParticle(muon); 
    muons.sort();
}

void Event::addPhoton(Photon photon) 
{
    photons.addParticle(photon); 
    photons.sort();
}

void Event::addJet(Jet jet) 
{
    jets.addParticle(jet); 
    jets.sort();
}

void Event::addSpecialObject(std::string key, Particle obj) 
{
    specialObjects[key].addParticle(obj); 
    specialObjects[key].sort();
}

ParticleCollection<Particle> Event::getSpecial(std::string key) const
{
    auto it = getSpecials().find(key);
    if (it == getSpecials().end()) // return null collection
    {
        auto nullcollection = ParticleCollection();
        return nullcollection;
    }
    else
    {
        return it->second;
    }
}

bool Event::containsParticles() const
{
    if (electrons.getParticles().size()==0 && muons.getParticles().size()==0 && photons.getParticles().size()==0 && jets.getParticles().size()==0 && specialObjects.size()==0)
    {
        return false;
    }
    return true;
}

void Event::clear()
{
    electrons = ParticleCollection<Electron>();
    muons = ParticleCollection<Muon>();
    photons = ParticleCollection<Photon>();
    jets = ParticleCollection<Jet>();
    specialObjects.clear();
    MET = 0.0f;
}
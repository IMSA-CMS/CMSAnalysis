#include "CMSAnalysis/Utility/interface/Event.hh"

#include <vector>
#include <unordered_map>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Jet.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Filters/interface/Cut.hh"


Event::Event()
{

}

ParticleCollection<Particle> Event::getParticles() const
{
    ParticleCollection<Particle> particleList;
   
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
        auto nullcollection = ParticleCollection<Particle>();
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
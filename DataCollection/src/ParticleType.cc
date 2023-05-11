#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"
#include "CMSAnalysis/DataCollection/interface/SingleParticleHist.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/CollectionHist.hh"

#include<string>
#include<vector>
#include<functional>

std::unordered_map<std::string,ParticleType> ParticleType::typeList = std::unordered_map<std::string,ParticleType>();

ParticleType::ParticleType(std::string typeName, int typepdgId, double typeCharge, std::vector<SingleParticleHist> typeParticleHists, std::vector<CollectionHist> typeCollectionHists)
{
    name = typeName;
    pdgId = typepdgId;
    charge = typeCharge;
    particleHists = typeParticleHists;
    collectionHists = typeCollectionHists;
}

std::vector<std::shared_ptr<SingleParticleHist>> ParticleType::getParticleHists() const
{
    std::vector<std::shared_ptr<SingleParticleHist>> newHists = std::vector<std::shared_ptr<SingleParticleHist>>{};
    for (auto hist : particleHists)
    {
        //histograms need to be cloned otherwise they would all point to the same object
        newHists.push_back(std::make_shared<SingleParticleHist>(hist.clone())); 
    }
    return newHists;
}

std::vector<std::shared_ptr<CollectionHist>> ParticleType::getCollectionHists() const
{
    std::vector<std::shared_ptr<CollectionHist>> newHists = std::vector<std::shared_ptr<CollectionHist>>{};
    for (auto hist : collectionHists)
    {
        //histograms need to be cloned otherwise they would all point to the same object
        newHists.push_back(std::make_shared<CollectionHist>(hist.clone())); 
    }
    return newHists;
}

const ParticleType& ParticleType::registerType(std::string typeName, int typepdgId, double typeCharge, std::vector<SingleParticleHist> typeParticleHists, std::vector<CollectionHist> typeCollectionHists)
{
    //particleTypes are not automatically stored, rather they are created the first time the function is called and then subsequently refrenced
    auto it = typeList.find(typeName);
    if (it == typeList.end())
    {
        ParticleType particleType = ParticleType(typeName,typepdgId,typeCharge,typeParticleHists,typeCollectionHists);
        typeList.insert({typeName, particleType});
    } 
    it = typeList.find(typeName);
    return it->second;
}


SingleParticleHist ParticleType::getPtHist()
{
    return SingleParticleHist("Pt", 150, 0, 1000, [](Particle particle){return std::vector<double>{particle.getPt()};});
}

SingleParticleHist ParticleType::getPhiHist()
{
    return SingleParticleHist("Phi", 150, -4, 4, [](Particle particle){return std::vector<double>{particle.getPhi()};});
}

SingleParticleHist ParticleType::getEtaHist()
{
    return SingleParticleHist("Eta", 150, -10, 10, [](Particle particle){return std::vector<double>{particle.getEta()};});
}

CollectionHist ParticleType::getNumberHist()
{
    return CollectionHist("Number", 11, -0.5, 10.5, [](std::shared_ptr<ParticleCollection<Particle>> collection){return std::vector<double>{collection->getNumParticles()};});
}

CollectionHist ParticleType::getSameSignInvariantMassHist()
{
    return CollectionHist("Same Sign Invariant Mass", 150, 0, 2000, [](std::shared_ptr<ParticleCollection<Particle>> collection){return std::vector<double>{collection->calculateSameSignInvariantMass()};});
}

CollectionHist ParticleType::getOppositeSignInvariantMassHist()
{
    return CollectionHist("Opposite Sign Invariant Mass", 150, 0, 1000, [](std::shared_ptr<ParticleCollection<Particle>> collection){return std::vector<double>{collection->calculateOppositeSignInvariantMass()};});
}

SingleParticleHist ParticleType::getDaughterDeltaRHist()
{
    return SingleParticleHist("Daughter Delta R", 150, 0, 3, [](Particle particle)
    {
        GenSimParticle genSimParticle = GenSimParticle(particle);
        if (genSimParticle.numberOfDaughters() == 2)
        {
            double deltaR = genSimParticle.daughter(0).getDeltaR(genSimParticle.daughter(1));
            return std::vector<double>{deltaR};
        }
        return std::vector<double>{};
    });
}

const ParticleType& ParticleType::electron()
{
    return registerType("Electron",11,-1,
    std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHist>{getNumberHist(),getSameSignInvariantMassHist(),getOppositeSignInvariantMassHist()}); 
}

const ParticleType& ParticleType::muon()
{
    return registerType("Muon",13,-1,
    std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHist>{getNumberHist(),getSameSignInvariantMassHist(),getOppositeSignInvariantMassHist()}); 
}

const ParticleType& ParticleType::tau()
{
    return registerType("Tau",15,-1,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::jet()
{
    return registerType("Jet",0,0,
    std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::leptonJet()
{
    return registerType("Lepton Jet",0,0,
    std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::photon()
{
    return registerType("Photon",22,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::darkPhoton()
{
    return registerType("Dark Photon",4900022,0,
    std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist(),getDaughterDeltaRHist()},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::neutralino()
{
    return registerType("Neutralino",1000022,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::leftDoublyHiggs()
{
    return registerType("Left Doubly Charged Higgs",9900041,2,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::rightDoublyHiggs()
{
    return registerType("Right Doubly Charged Higgs",9900042,2,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::z()
{
    return registerType("Z Boson",23,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::w()
{
    return registerType("W Boson",24,1,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::higgs()
{
    return registerType("Higgs Boson",25,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
}

const ParticleType& ParticleType::meson() //place holder because we don't care (same for mesons, jets, leptonjets and none)
{
    return registerType("Meson",0,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
} 

const ParticleType& ParticleType::baryon()
{
    return registerType("Baryon",0,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{getNumberHist()}); 
} 

const ParticleType& ParticleType::none()
{
    return registerType("None",0,0,
    std::vector<SingleParticleHist>{},
    std::vector<CollectionHist>{}); 
}

bool ParticleType::operator== (const ParticleType type) const
{
    //necessary to directly compare particleTypes
    if (type.getName() == getName())
    {
        return true;
    }
    return false;
}

bool ParticleType::operator!= (const ParticleType type) const
{
    //necessary to directly compare particleTypes
    if (type.getName() != getName())
    {
        return true;
    }
    return false;
}
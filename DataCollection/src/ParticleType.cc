#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"
#include "CMSAnalysis/DataCollection/interface/SingleParticleHist.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"

#include<string>
#include<vector>
#include<functional>

std::unordered_map<std::string,ParticleType> ParticleType::typeList = std::unordered_map<std::string,ParticleType>();

ParticleType::ParticleType(std::string typeName, int typepdgId, std::vector<SingleParticleHist> typeHists)
{
    name = typeName;
    pdgId = typepdgId;
    hists = typeHists;
}

std::vector<std::shared_ptr<SingleParticleHist>> ParticleType::getHists() const
{
    std::vector<std::shared_ptr<SingleParticleHist>> newHists = std::vector<std::shared_ptr<SingleParticleHist>>{};
    for (auto hist : hists)
    {
        //histograms need to be cloned otherwise they would all point to the same object
        newHists.push_back(std::make_shared<SingleParticleHist>(hist.clone())); 
    }
    return newHists;
}

const ParticleType& ParticleType::registerType(std::string typeName, int typepdgId, std::vector<SingleParticleHist> typeHists)
{
    //particleTypes are not automatically stored, rather they are created the first time the function is called and then subsequently refrenced
    auto it = typeList.find(typeName);
    if (it == typeList.end())
    {
        ParticleType particleType = ParticleType(typeName,typepdgId,typeHists);
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

SingleParticleHist ParticleType::getDaughterDeltaRHist()
{
    return SingleParticleHist("Daughter DeltaR", 150, 0, 3, [](Particle particle)
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
    return registerType("Electron",11,std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist()}); 
}

const ParticleType& ParticleType::muon()
{
    return registerType("Muon",13,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::jet()
{
    return registerType("Jet",0,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::leptonJet()
{
    return registerType("Lepton Jet",0,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::photon()
{
    return registerType("Photon",22,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::darkPhoton()
{
    return registerType("Dark Photon",4900022,std::vector<SingleParticleHist>{getPtHist(),getPhiHist(),getEtaHist(),getDaughterDeltaRHist()}); 
}

const ParticleType& ParticleType::neutralino()
{
    return registerType("Neutralino",1000022,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::leftDoublyHiggs()
{
    return registerType("Left Doubly Charged Higgs",9900041,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::rightDoublyHiggs()
{
    return registerType("Right Doubly Charged Higgs",9900042,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::zBoson()
{
    return registerType("Z Boson",23,std::vector<SingleParticleHist>{}); 
}

const ParticleType& ParticleType::none()
{
    return registerType("None",0,std::vector<SingleParticleHist>{}); 
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
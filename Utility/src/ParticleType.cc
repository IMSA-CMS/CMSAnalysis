#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/HistParams.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"

#include <fstream>
#include <string>
#include <vector>
#include <functional>

std::unordered_map<int,ParticleType> ParticleType::typeList = std::unordered_map<int,ParticleType>();

ParticleType::ParticleType() {}

ParticleType::ParticleType(std::string typeName, int typepdgId, double typeCharge, std::vector<HistParams> typeParticleHists, std::vector<CollectionHistParams> typeCollectionHists)
{
    name = typeName;
    pdgId = typepdgId;
    charge = typeCharge;
    particleHists = typeParticleHists;
    collectionHists = typeCollectionHists;
}

std::vector<HistParams> ParticleType::getParticleHists() const
{
    return particleHists;
}

std::vector<CollectionHistParams> ParticleType::getCollectionHists() const
{
    /*std::vector<std::shared_ptr<CollectionHistParams>> newHists = std::vector<std::shared_ptr<CollectionHistParams>>{};
    for (auto hist : CollectionHistParams)
    {
        //histograms need to be cloned otherwise they would all point to the same object
        newHists.push_back(std::make_shared<CollectionHistParams>(hist.clone())); 
    }*/
    return collectionHists;
}

bool ParticleType::isQuark() const
{
   int absID = std::abs(pdgId);
   return absID<10; 
}

bool ParticleType::isMeson() const
{
    int absID = std::abs(pdgId);
    return absID<1000 && absID>100;
}

bool ParticleType::isBaryon() const
{
    int absID = std::abs(pdgId);
    return 1000<absID && 10000>absID;
}

bool ParticleType::isHadron() const
{
    return isMeson() || isBaryon();
}

const ParticleType& ParticleType::registerType(std::string typeName, int typepdgId, double typeCharge, std::vector<HistParams> typeParticleHists, std::vector<CollectionHistParams> typeCollectionHistParamss)
{
    //particleTypes are not automatically stored, rather they are created the first time the function is called and then subsequently refrenced
    auto it = typeList.find(typepdgId);
    if (it == typeList.end())
    {
        ParticleType particleType = ParticleType(typeName,typepdgId,typeCharge,typeParticleHists,typeCollectionHistParamss);
        typeList.insert({typepdgId, particleType});
    } 

    return getPDGType(typepdgId);
}


bool ParticleType::loadParticle(std::ifstream& file)
{
    char tempString[100];
    int pdgID;
    std::string name;
    double chargeType;

    file.ignore(100, '\"');
    file.getline(tempString, 100, '\"');

    pdgID = std::stoi(std::string(tempString));

    file.ignore(100, '\"');
    file.getline(tempString, 100, '\"');

    name = std::string(tempString);

    //Skip to chargeType
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');

    file.getline(tempString, 100, '\"');
    chargeType = std::stoi(std::string(tempString)) / 3.0;

    std::vector<HistParams> defaultHistParams = {getPtHist(),getPhiHist(),getEtaHist()};
    std::vector<CollectionHistParams> defaultCollectionHistParams = {getNumberHist(), getSameSignInvariantMassHist(), getOppositeSignInvariantMassHist()}; 
    
    registerType(name, pdgID, chargeType, defaultHistParams, defaultCollectionHistParams);


    return true;
}


bool ParticleType::loadParticleDatabase(const std::string& fileName)
{
    std::ifstream file(fileName);

    if (!file)
    {
        std::cout << fileName << " could not be opened!\n";
        return false;
    }

    for (int i = 0; true; ++i)
    {
        //std::cout << "Iteration number " << i << "\n";
        char peekedChar = file.peek();

        //std::cout << "Peeked char = " << peekedChar << "\n";

        if (peekedChar == 'p')
        {
            //Adds particle to map
            loadParticle(file);
            //std::cout << "Particle key " << i << "\n";
        }
        else if (peekedChar == EOF)
        {
            //std::cout << "End of file reached\n";
            break;
        }
        file.ignore(100, '\n');
        
    }
    particleTypeOverrides();
    return true;
}

const ParticleType& ParticleType::getPDGType(int pdgID)
{
    int absolutePdgID = std::abs(pdgID);
    // Automatically load particle database if typeList is empty
    if (typeList.empty()) 
        ParticleType::loadParticleDatabase(Utility::getFullPath("ParticleData.txt"));

    if (typeList.find(absolutePdgID) != typeList.end())
        return typeList[absolutePdgID];
    else
        return typeList[0];
}

void ParticleType::particleTypeOverrides()
{
    typeList[11].collectionHists.push_back(getSameSignInvariantMassHist());
    typeList[11].collectionHists.push_back(getOppositeSignInvariantMassHist());

    typeList[13].collectionHists.push_back(getSameSignInvariantMassHist());
    typeList[13].collectionHists.push_back(getOppositeSignInvariantMassHist());

    typeList[15].collectionHists.push_back(getSameSignInvariantMassHist());
    typeList[15].collectionHists.push_back(getOppositeSignInvariantMassHist());

    registerType("Meson",26,0,
    std::vector<HistParams>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHistParams>{getNumberHist()}); 

    registerType("Baryon",27,0,
    std::vector<HistParams>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHistParams>{getNumberHist()}); 

    registerType("Jet",28,0,
    std::vector<HistParams>{getPtHist(),getPhiHist(),getEtaHist()},
    std::vector<CollectionHistParams>{getNumberHist()}); 

    registerType("Lepton Jet",29,0,
    std::vector<HistParams>{getPtHist(),getPhiHist(),getEtaHist(), getLeptonJetDeltaRHist(), getLeptonJetMassHist(),
        getLeptonJetMassHistZoomed()},
    std::vector<CollectionHistParams>{getNumberHist()}); 
}

HistParams ParticleType::getPtHist()
{
    return HistParams("Pt", 150, 0, 1000, [](Particle particle){return std::vector<double>{particle.getPt()};});
}



HistParams ParticleType::getPhiHist()
{
    return HistParams("Phi", 150, -4, 4, [](Particle particle){return std::vector<double>{particle.getPhi()};});
}

CollectionHistParams ParticleType::getNumberHist()
{
    return CollectionHistParams("Number", 11, -0.5, 10.5, [](std::shared_ptr<ParticleCollection<Particle>> collection){return std::vector<double>{collection->getNumParticles()};});
}

CollectionHistParams ParticleType::getSameSignInvariantMassHist()
{
    return CollectionHistParams("Same Sign Invariant Mass", 150, 0, 2000, calcSameSignInvariantMass);
}

std::vector<double> ParticleType::calcSameSignInvariantMass(std::shared_ptr<ParticleCollection<Particle>> collection)
{
    double invariantMass = collection->calculateSameSignInvariantMass(true);
    if (invariantMass > 0)
    {
        return {invariantMass};
    }
    else
    {
        return {};
    }
}

CollectionHistParams ParticleType::getOppositeSignInvariantMassHist()
{
    return CollectionHistParams("Opposite Sign Invariant Mass", 150, 0, 1000, calcOppositeSignInvariantMass);
}

std::vector<double> ParticleType::calcOppositeSignInvariantMass(std::shared_ptr<ParticleCollection<Particle>> collection)
{
    double invariantMass = collection->calculateOppositeSignInvariantMass(true);
    if (invariantMass > 0)
    {
        return {invariantMass};
    }
    else
    {
        return {};
    }
}

HistParams ParticleType::getEtaHist()
{
    return HistParams("Eta", 150, -3, 3, [](Particle particle){return std::vector<double>{particle.getEta()};});
}

HistParams ParticleType::getDaughterDeltaRHist()
{
    return HistParams("Daughter Delta R", 150, 0, 3, [](Particle particle)
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

HistParams ParticleType::getLeptonJetDeltaRHist()
{
   return HistParams("Lepton Jet Delta R", 100, 0, 0.5, [](Particle particle){
    auto leptonJet = LeptonJet(particle);
    return std::vector<double>{leptonJet.getDeltaR()};
    });
}

HistParams ParticleType::getLeptonJetMassHist()
{
    return HistParams("Lepton Jet Mass", 100, 0, 100, [](Particle particle){
    auto leptonJet = LeptonJet(particle);
    return std::vector<double>{leptonJet.getMass()};
    });
}

HistParams ParticleType::getLeptonJetMassHistZoomed()
{
    return HistParams("Lepton Jet Mass Zoom", 100, 0, 10, [](Particle particle){
    auto leptonJet = LeptonJet(particle);
    return std::vector<double>{leptonJet.getMass()};
    });
}

const ParticleType& ParticleType::electron()
{
    return getPDGType(11);
}

const ParticleType& ParticleType::muon()
{
    return getPDGType(13);
}

const ParticleType& ParticleType::tau()
{
    return getPDGType(15);
}

const ParticleType& ParticleType::jet()
{
    return getPDGType(28);
}

const ParticleType& ParticleType::leptonJet()
{
    return getPDGType(29);
}

const ParticleType& ParticleType::photon()
{
    return getPDGType(22);
}

const ParticleType& ParticleType::down()
{
    return getPDGType(1);
}

const ParticleType& ParticleType::up()
{
    return getPDGType(2);
}

const ParticleType& ParticleType::strange()
{
    return getPDGType(3);
}

const ParticleType& ParticleType::charm()
{
    return getPDGType(4);
}

const ParticleType& ParticleType::bottom()
{
    return getPDGType(5);
}

const ParticleType& ParticleType::top()
{
    return getPDGType(6);
}

const ParticleType& ParticleType::darkPhoton()
{
    return getPDGType(4900022);
}

const ParticleType& ParticleType::neutralino()
{
    return getPDGType(1000022);
}

const ParticleType& ParticleType::leftDoublyHiggs()
{
    return getPDGType(9900041);
}

const ParticleType& ParticleType::rightDoublyHiggs()
{
    return getPDGType(9900042);
}

const ParticleType& ParticleType::z()
{
    return getPDGType(23);
}

const ParticleType& ParticleType::w()
{
    return getPDGType(24);
}

const ParticleType& ParticleType::higgs()
{
    return getPDGType(25);
}

const ParticleType& ParticleType::meson() //place holder because we don't care (same for mesons, jets, leptonjets and none)
{
    return getPDGType(26);
} 

const ParticleType& ParticleType::baryon()
{
    return getPDGType(26);
} 

const ParticleType& ParticleType::none()
{
    return getPDGType(0);
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
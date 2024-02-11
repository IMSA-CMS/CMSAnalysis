#ifndef PARTICLETYPE_HH
#define PARTICLETYPE_HH

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "HistParams.hh"
#include "Particle.hh"
#include "CollectionHistParams.hh"

//ParticleType object which may be assigned to a particle. 
//All usages of ParticleType are refrences to the "typeList" variable.
//To access any of the types call the static member function associated with that particle.
//The type of a Particle is identified in the getPDGType() function in Particle.cc
//Particle types have hists attached to them which are used in EventModule
class ParticleType
{
    public:
    std::string getName() const {return name;}
    int getpdgId() const { return pdgId;}
    double getCharge() const {return charge;}
    std::vector<HistParams> getParticleHists() const;
    std::vector<CollectionHistParams> getCollectionHists() const;

    static const ParticleType& getPDGType(int pdgID);

    //Member function used to check type
    static const ParticleType& electron();
    static const ParticleType& muon();
    static const ParticleType& jet(); 
    static const ParticleType& tau();
    static const ParticleType& leptonJet(); 
    static const ParticleType& up();
    static const ParticleType& down();
    static const ParticleType& strange();
    static const ParticleType& charm();
    static const ParticleType& top();
    static const ParticleType& bottom();
    static const ParticleType& photon();
    static const ParticleType& darkPhoton();
    static const ParticleType& neutralino();
    static const ParticleType& leftDoublyHiggs();
    static const ParticleType& rightDoublyHiggs();
    static const ParticleType& z();
    static const ParticleType& w();
    static const ParticleType& higgs();
    static const ParticleType& meson();
    static const ParticleType& baryon();

    static const ParticleType& none();

    static bool loadParticleDatabase(const std::string& fileName);

    //Default constructor necessary for std::unordered_map for allocating new elements with [] operator
    ParticleType();
    ParticleType(std::string typeName, int typepdgId, double charge, std::vector<HistParams> typeParticleHists, std::vector<CollectionHistParams> typeCollectionHists); //std::vector<CollectionHistParams> CollectionHistParamss);
    
    bool operator== (const ParticleType type) const;
    bool operator!= (const ParticleType type) const;

    private:
    std::string name;
    int pdgId;
    double charge;
    std::vector<HistParams> particleHists;
    std::vector<CollectionHistParams> collectionHists;

    //Avaliable hists for ParticleTypes
    static HistParams getPtHist();
    static HistParams getPhiHist();
    static HistParams getEtaHist();
    static HistParams getDaughterDeltaRHist();

    static CollectionHistParams getNumberHist();
    static CollectionHistParams getSameSignInvariantMassHist();
    static CollectionHistParams getOppositeSignInvariantMassHist();

    static bool loadParticle(std::ifstream& file);
    //Lets you manually change the particle types in the method definition
    static void particleTypeOverrides();
    //Should probably be changed to void and replaced with getPDGType()
    static const ParticleType& registerType(std::string typeName, int typepdgId, double charge, std::vector<HistParams> typeParticleHists, std::vector<CollectionHistParams> typeCollectionHistParams);//, std::vector<CollectionHistParams> CollectionHistParams);

    static std::unordered_map<int, ParticleType> typeList;

    //Adds type object to list if it does not exist and then refrence it
};

#endif
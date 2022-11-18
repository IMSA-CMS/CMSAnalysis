#ifndef PARTICLETYPE_HH
#define PARTICLETYPE_HH

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "SingleParticleHist.hh"
#include "Particle.hh"


//ParticleType object which may be assigned to a particle. 
//All usages of ParticleType are refrences to the "typeList" variable.
//To access any of the types call the static member function associated with that particle.
//The type of a Particle is identified in the identifyType() function in Particle.cc
//Particle types have hists attached to them which are used in EventModule
class ParticleType
{
    public:
    std::string getName() const {return name;};
    int getpdgId() const {return pdgId;};
    std::vector<std::shared_ptr<SingleParticleHist>> getHists() const;

    //Member function used to check type
    static const ParticleType& electron();
    static const ParticleType& muon();
    static const ParticleType& jet(); 
    static const ParticleType& leptonJet(); 
    static const ParticleType& photon();
    static const ParticleType& darkPhoton();
    static const ParticleType& neutralino();
    static const ParticleType& leftDoublyHiggs();
    static const ParticleType& rightDoublyHiggs();
    static const ParticleType& zBoson();
    static const ParticleType& none();

    bool operator== (const ParticleType type) const;
    bool operator!= (const ParticleType type) const;

    private:
    std::string name;
    int pdgId;
    std::vector<SingleParticleHist> hists;

    //Avaliable hists for ParticleTypes
    static SingleParticleHist getPtHist();
    static SingleParticleHist getPhiHist();
    static SingleParticleHist getEtaHist();
    static SingleParticleHist getDaughterDeltaRHist();

    ParticleType(std::string typeName, int typepdgId, std::vector<SingleParticleHist> typeHists);

    static std::unordered_map<std::string,ParticleType> typeList;

    //Adds type object to list if it does not exist and then refrence it
    static const ParticleType& registerType(std::string typeName, int typepdgId, std::vector<SingleParticleHist>  typeHists);
};

#endif
#ifndef GENSIMPARTICLE_HH
#define GENSIMPARTICLE_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

class LeptonJet;

//Used for accessing generation data from a particle. 
//GenSimParticle has access to data not available in Particle such as PDGID, and lineage.
class GenSimParticle : public Particle
{
    public:
        using Particle::Particle;
        GenSimParticle(const Particle&);

        int pdgId() const;

        //status is an internal pythia value which refers to the particle's state
        int status() const;

        GenSimParticle mother() const;
        bool hasMother();
        int numberOfDaughters() const;
        GenSimParticle daughter(int i) const;
        std::vector<GenSimParticle> getDaughters() const;
        bool isFinalState() const;

        //finds the first mother which is not itself
        GenSimParticle uniqueMother() const;

        //finds the last hit of a particle before it decays into something else
        GenSimParticle finalDaughter() const;
        bool isFinalDaughter() {return finalDaughter() == *this;}

        GenSimParticle findMother(int motherPDGID) const;

        //finds the latest particle which is a mother of 2 or more particles
        static GenSimParticle sharedMother(int motherPDGID, GenSimParticle particle1, GenSimParticle particle2);
        static GenSimParticle sharedMother(int motherPDGID, std::vector<Particle> particles);


};

std::ostream& operator<<(std::ostream& str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles);
std::ostream& operator<<(std::ostream& str, const GenSimParticle part);

#endif

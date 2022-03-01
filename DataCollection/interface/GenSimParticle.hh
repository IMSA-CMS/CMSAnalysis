#ifndef GENSIMPARTICLE_HH
#define GENSIMPARTICLE_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class LeptonJet;


class GenSimParticle : public Particle
{
    public:
        using Particle::Particle;

        int pdgId() const;
        int status() const;
        GenSimParticle mother() const;
        int numberOfDaughters() const;
        GenSimParticle daughter(int i) const;
        bool isFinalState() const;
        GenSimParticle uniqueMother() const;
        GenSimParticle finalDaughter() const;
        GenSimParticle findMother(int motherPDGID) const;
        static GenSimParticle sharedMother(int motherPDGID, Particle particle1, Particle particle2);
        static GenSimParticle sharedMother(int motherPDGID, std::vector<Particle> particles);


};
#endif
#ifndef RECOPARTICLE_HH
#define RECOPARTICLE_HH

#include "Particle.hh"
class RecoParticle: public Particle 
{
public:
using Particle::Particle;
RecoParticle(const Particle& particle);
};

#endif
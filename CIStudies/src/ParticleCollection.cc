#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"

#include <cmath>
#include "TLorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <iostream>

using Particle = const reco::Candidate*;
using PartPair = std::pair<Particle, Particle>;

double ParticleCollection::getInvariantMass() const
{
  auto particlePair = chooseParticles();
  if (particlePair.first && particlePair.second)
    {
      
      return calculateInvariantMass(particlePair.first, particlePair.second);
    }

  else
    {
      
      return -1;
    }
}

double ParticleCollection::getLeadingTransverseMomentum() const
{
  auto particlePair = chooseParticles();
  if (particlePair.first && particlePair.second)
    {
      
      return calculateLeadingTransverseMomentum(particlePair.first, particlePair.second);
    }

  else
    {
      
      return -1;
    }
}

double ParticleCollection::getCollinsSoper() const
{
  auto particlePair = chooseParticles();

  //guarantees that the particle has to have a negative charge or the antiparticle has to have a positve charge
  //if both particles have the same sign (i.e. electrons), the situation is solved at the start of calculateCollinsSoper()
  if (particlePair.first && particlePair.second)
    {
      if (particlePair.first->charge() < 0) //no flip
	{
	  
	  return calculateCollinsSoper(particlePair.first, particlePair.second);
	}
      else //flip
	{
	  
	  return calculateCollinsSoper(particlePair.second, particlePair.first);
	}
    }
  else
    {
      
      return -2;
    }
}

PartPair ParticleCollection::chooseParticles() const
{
  

  double maxInvariantMass = 0;
  bool oppositeSigns = false;

  const reco::Candidate* iPointer = nullptr;
  const reco::Candidate* jPointer = nullptr;

  for (int i = 0; i < static_cast<int>(particles.size()) - 1; ++i)
    {
      for (int j = i + 1; j < static_cast<int>(particles.size()); ++j)
	{
	  if (checkSigns(particles[i], particles[j]))
	    {
	      oppositeSigns = true;
	      if (calculateInvariantMass(particles[i], particles[j]) > maxInvariantMass)
		{
		  maxInvariantMass = calculateInvariantMass(particles[i], particles[j]);
		  iPointer = particles[i];
		  jPointer = particles[j];
		}
	    }

	  else if (!oppositeSigns)
	    {
	      if (calculateInvariantMass(particles[i], particles[j]) > maxInvariantMass)
		{
		  maxInvariantMass = calculateInvariantMass(particles[i], particles[j]);
		  iPointer = particles[i];
		  jPointer = particles[j];
		}
	    }
	}
    }

  
  return {iPointer, jPointer};
}

//returns true if particle1 and particle2 have opposite signs
bool ParticleCollection::checkSigns(Particle particle1, Particle particle2) const
{
  if (particle1->charge() != particle2->charge())
    {
      return true;
    }

  return false;
}

double ParticleCollection::calculateInvariantMass(Particle particle1, Particle particle2) const
{
  double product = 2 * particle1->pt() * particle2->pt(); 
  double diff = cosh(particle1->eta() - particle2->eta()) - cos(particle1->phi() - particle2->phi()); 
  double invariantMass = product * diff; 
  if (invariantMass > 0)
    {
      return sqrt(invariantMass);
    }
  else
    {
      return 0; 
    }
}

double ParticleCollection::calculateLeadingTransverseMomentum(Particle particle1, Particle particle2) const
{
  double pt1 = particle1->pt();
  double pt2 = particle2->pt();
  // Returns the highest transverse momentum (pt)
  if (pt1 > pt2)
   {
      return pt1;
   }
  else
   {
      return pt2;
   }
}

double ParticleCollection::calculateCollinsSoper(Particle particle, Particle antiparticle) const
{
  //if same-sign problem occurs, we trust the charge of the particle with abs(eta) closer to 0
  if (!checkSigns(particle, antiparticle) && lowEtaFlip(particle, antiparticle))
    {
      Particle temp = particle;
      particle = antiparticle;
      antiparticle = temp;
    }

  TLorentzVector Ele;
  TLorentzVector Elebar;

  float Et1 = particle->et();
  float Et2 = antiparticle->et();
  float Eta1 = particle->eta();
  float Eta2 = antiparticle->eta();
  float Phi1 = particle->phi();
  float Phi2 = antiparticle->phi();
  float En1 = particle->energy();
  float En2 = antiparticle->energy();
  Ele.SetPtEtaPhiE(Et1,Eta1,Phi1,En1);
  Elebar.SetPtEtaPhiE(Et2,Eta2,Phi2,En2);

  return calculateCosTheta(Ele, Elebar);
}

double ParticleCollection::calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const
{
  double Eleplus  = 1.0/sqrt(2.0) * (Ele.E() + Ele.Z());
  double Eleminus = 1.0/sqrt(2.0) * (Ele.E() - Ele.Z());

  double Elebarplus  = 1.0/sqrt(2.0) * (Elebar.E() + Elebar.Z());
  double Elebarminus = 1.0/sqrt(2.0) * (Elebar.E() - Elebar.Z());

  TLorentzVector Q(Ele+Elebar);

 
  double costheta = 2.0 / (Q.Mag() * sqrt(pow(Q.Mag(),2) + pow(Q.Pt(),2))) * (Eleplus * Elebarminus - Eleminus * Elebarplus);
  if (Q.Pz() < 0) costheta = -costheta;
  return costheta;
}

bool ParticleCollection::lowEtaFlip(Particle particle, Particle antiparticle) const
{
  if (std::abs(particle->eta()) < std::abs(antiparticle->eta()))
    {
      if (particle->charge() < 0)
	{
	  return true;
	}
    }

  else 
    {
      if (antiparticle->charge() > 0)
	{
	  return true;
	}
    }

  return false;
}



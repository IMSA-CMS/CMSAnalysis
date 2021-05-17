#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"

#include <cmath>
#include "TLorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CIAnalysis/CIStudies/interface/Particle.hh"

#include <iostream>

//using Particle = const reco::Candidate*;
using PartPair = std::pair<Particle, Particle>;

double ParticleCollection::getInvariantMass() const
{
  auto particlePair = chooseParticles();
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
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
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {

      return calculateLeadingTransverseMomentum(particlePair.first, particlePair.second);
    }
  else
    {

      return -1;
    }
}

double ParticleCollection::getNthHighestPt(int n) const
{
  if (n < 1)
  {
    throw std::invalid_argument("Not a positive integer");
  }

  if (n > getNumParticles())
  {
    return 0;   // If the nth highest pt particle doesn't exist, return 0
  }

  auto particlesVec = getParticles();  // Vector of Particles

  // Sort the vector of particles by pt (greatest to least)
  std::sort(particlesVec.begin(), particlesVec.end(), [](auto a, auto b){return a.pt() > b.pt();});

  // for (auto particle : particlesVec)
  // {
  //  std::cout << particle.pt() << '\n';
  // }
  // std::cout << '\n';

  return particlesVec[n - 1].pt();  // n-1 since the first element is 0, 2nd element is 1, etc.
}

double ParticleCollection::getLeadingPt() const
{
  double highestPt = 0;
  for (auto particle : particles)
  {
    double pt = particle.pt();
    if (pt > highestPt)
    {
      pt = highestPt;
    }
  }

  return highestPt;
}

Particle ParticleCollection::getLeadingPtLepton() const
{
  double highestPt = getLeadingPt();
  for (auto part : particles)
  {
    if (part.pt() == highestPt)
    {
      return part;
    }
  }
   
  throw std::runtime_error("No leading pT lepton!");
}

double ParticleCollection::calculateLeadingTransverseMomentum(Particle particle1, Particle particle2) const
{
  double pt1 = particle1.pt();
  double pt2 = particle2.pt();
  if (pt1 > pt2)
    {
      return pt1;
    }
  else
    {
      return pt2;
    }
}

double ParticleCollection::getCollinsSoper() const
{
  auto particlePair = chooseParticles();

  //guarantees that the particle has to have a negative charge or the antiparticle has to have a positve charge
  //if both particles have the same sign (i.e. electrons), the situation is solved at the start of calculateCollinsSoper()
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {
      if (particlePair.first.charge() < 0) //no flip
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

bool ParticleCollection::isBB() const
{
  auto particlePair = chooseParticles();
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {
      if (particlePair.first.getBarrelState() == Particle::BarrelState::Barrel && particlePair.second.getBarrelState() == Particle::BarrelState::Barrel)
	{
	  return true;
	}
    }
  return false;
}

bool ParticleCollection::isBE() const
{
  auto particlePair = chooseParticles();
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {
      if ((particlePair.first.getBarrelState() == Particle::BarrelState::Barrel
	   && particlePair.second.getBarrelState() == Particle::BarrelState::Endcap)
	   || (particlePair.first.getBarrelState() == Particle::BarrelState::Endcap 
	   && particlePair.second.getBarrelState() == Particle::BarrelState::Barrel))
	{
	  return true;
	}
      //if both particles are muons, then EE counts as BE
      if ((particlePair.first.getLeptonType() == Particle::LeptonType::Muon
	   && particlePair.second.getLeptonType() == Particle::LeptonType::Muon)
	   && (particlePair.first.getBarrelState() == Particle::BarrelState::Endcap
	   && particlePair.second.getBarrelState() == Particle::BarrelState::Endcap))
	{
	  return true;
	}
    }
  return false;
}

int ParticleCollection::getLeptonTypeCount(Particle::LeptonType leptonType) const  // Finds the number of a certain lepton type (electrons/muons)
{
  int count = 0;
  for (auto particle : particles)
  {
    if (particle.getLeptonType() == leptonType)
    {
      ++count;
    }
  }

  return count;
}

PartPair ParticleCollection::chooseParticles() const
{
  auto particlePair = chooseParticles(true); // opposite signs
  
  if (!(particlePair.first.isNotNull() && particlePair.second.isNotNull()))  // If the particle pair is empty, then test same signs
  {
    particlePair = chooseParticles(false);
  }

  return particlePair;
}

PartPair ParticleCollection::chooseParticles(bool oppositeSigns) const
{
  double maxInvariantMass = 0;
  Particle iPointer(nullptr, Particle::LeptonType::None);
  Particle jPointer(nullptr, Particle::LeptonType::None);

  for (int i = 0; i < static_cast<int>(particles.size()) - 1; ++i)
    {
      for (int j = i + 1; j < static_cast<int>(particles.size()); ++j)
	{
	  if (checkSigns(particles[i], particles[j]) == oppositeSigns)     // Check if the particle pairs' signs match with what we want
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
  if (particle1.charge() != particle2.charge())
    {
      return true;
    }

  return false;
}

double ParticleCollection::calculateInvariantMass(Particle particle1, Particle particle2) const
{
  auto vec1 = particle1.fourVector();
  auto vec2 = particle2.fourVector();

  auto sum = vec1 + vec2;

  // std::cout << sum.M() << '\n';

  return sum.M();

  //double product = 2 * particle1.pt() * particle2.pt(); 
  //double diff = std::cosh(particle1.eta() - particle2.eta()) - std::cos(particle1.phi() - particle2.phi()); 
  //double invariantMass = product * diff; 
  //if (invariantMass > 0)
  //  {
  //    return std::sqrt(invariantMass);
  //  }
  //else
  //  {
  //    return 0; 
  //  }
}

double ParticleCollection::calculateSameSignInvariantMass() const
{
  auto particlePair = chooseParticles(false);					// we want same sign particles with highest invariant mass
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {      
      return calculateInvariantMass(particlePair.first, particlePair.second);
    }
  else
    {     
      return -1;
    }
}

double ParticleCollection::calculateOppositeSignInvariantMass() const
{
  auto particlePair = chooseParticles(true);					// we want opposite sign particles with highest invariant mass
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
    {      
      return calculateInvariantMass(particlePair.first, particlePair.second);
    }
  else
    {     
      return -1;
    }
}

double ParticleCollection::calculateAllLeptonInvariantMass() const
{
  reco::Candidate::LorentzVector total;

  for (auto particle : particles)
  {
    auto newVec = particle.fourVector();
    total += newVec;
  }

  // std::cout << total.M() << '\n';

  return total.M();
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

  float Et1 = particle.et();
  float Et2 = antiparticle.et();
  float Eta1 = particle.eta();
  float Eta2 = antiparticle.eta();
  float Phi1 = particle.phi();
  float Phi2 = antiparticle.phi();
  float En1 = particle.energy();
  float En2 = antiparticle.energy();
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
  if (std::abs(particle.eta()) < std::abs(antiparticle.eta()))
    {
      if (particle.charge() < 0)
	{
	  return true;
	}
    }

  else 
    {
      if (antiparticle.charge() > 0)
	{
	  return true;
	}
    }

  return false;
}



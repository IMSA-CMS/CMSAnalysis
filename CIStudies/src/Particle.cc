#include "CIAnalysis/CIStudies/interface/Particle.hh"


Particle::Particle(const reco::Candidate* iparticle, LeptonType iLeptonType): 
particle(iparticle),
leptonType(iLeptonType)
{
}

int Particle::charge() const 
{
  checkIsNull();
  return particle->charge();
}

double Particle::pt() const
{
  checkIsNull();
  return particle->pt();
}

double Particle::eta() const
{
  checkIsNull();
  return particle->eta();
}

double Particle::phi() const
{
  checkIsNull();
  return particle->phi();
}

double Particle::et() const
{
  checkIsNull();
  return particle->et();
}

double Particle::energy() const
{
  checkIsNull();
  return particle->energy();
}

int Particle::pdgId() const
{
  checkIsNull();
  return particle->pdgId();
}

int Particle::status() const
{
  checkIsNull();
  return particle->status();
}

Particle Particle::mother() const
{
  checkIsNull();
  //mother of particle is often not electron/muon
  return Particle(particle->mother(), Particle::LeptonType::None);
}

Particle::LeptonType Particle::getLeptonType() const
{
  checkIsNull();
  return leptonType;
}

Particle::BarrelState Particle::getBarrelState() const
{
  checkIsNull();
  double etaValue = std::abs(eta());
  if (leptonType == LeptonType::Muon)
  {
    if (etaValue < 1.2)
    {
      return Particle::BarrelState::Barrel;
    }
    else
    {
      return Particle::BarrelState::Endcap;
    }
  }
  else if (leptonType == LeptonType::Electron)
  {
    if (etaValue < 1.4442)
    {
      return Particle::BarrelState::Barrel;
    }
    else if (etaValue > 1.562)
    {
      return Particle::BarrelState::Endcap;
    }
  }
  return Particle::BarrelState::None;
}

void Particle::checkIsNull() const
{
  if(!particle)
  {
    throw std::runtime_error("attempted to use null pointer in Particle");	
  }
}

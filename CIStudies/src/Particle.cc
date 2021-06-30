#include "CIAnalysis/CIStudies/interface/Particle.hh"


Particle::Particle(const reco::Candidate* iparticle)://, LeptonType iLeptonType): 
particle(iparticle)
{}
//leptonType(iLeptonType)

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

reco::Candidate::LorentzVector Particle::fourVector() const
{
  checkIsNull();
  return particle->p4();
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
  return Particle(particle->mother());
}

Particle Particle::uniqueMother() const
{
  checkIsNull();
  // The mother that is not itself
  auto mom = mother();
  mom.checkIsNull();

  if (mom.pdgId() == pdgId())
  {
    return mom.uniqueMother(); //Recursive back to itself, so it will keep going until it returns a mother that is not the particle
  }

  return mom;
}

Particle::LeptonType Particle::getLeptonType() const //changed code
{
  checkIsNull();

  if (dynamic_cast<const reco::Muon*>(particle))
  {
    return LeptonType::Muon;
  }

  else if (dynamic_cast<const reco::Electron*>(particle))
  {
    return LeptonType::Electron;
  }

  else
  {
    return LeptonType::None;
  }
}

Particle::BarrelState Particle::getBarrelState() const
{
  checkIsNull();
  double etaValue = std::abs(eta());
  if (particle->getLeptonType() == LeptonType::Muon) //changed code
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
  else if (particle->getLeptonType() == LeptonType::Electron) //changed code
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


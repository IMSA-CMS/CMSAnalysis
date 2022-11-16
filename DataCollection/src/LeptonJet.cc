#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

LeptonJet::LeptonJet():
Particle(std::make_shared<LeptonJetImplementation>()){
  
}

LeptonJet::LeptonJet (const Particle& particle):
Particle(particle){}



bool LeptonJet::operator==(LeptonJet userJet) const
{
  return cast() == userJet.getParticleImplementation();
}

std::shared_ptr<LeptonJetImplementation> LeptonJet::cast() const
{
  auto pointer = std::dynamic_pointer_cast<LeptonJetImplementation>(getParticle());
  if(pointer)
  {
    return pointer;
  }
  else
  {
    throw std::runtime_error("Null pointer");
  }
}
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

LeptonJet::LeptonJet(const Particle particle)
{
  addParticle(particle);
}

std::vector<double> LeptonJet::getPt() const
{
  findPt();
  return ptVector;
}

std::vector<double> LeptonJet::getPhi() const
{
  findPhi();
  return phiVector;
}

std::vector<double> LeptonJet::getEta() const
{
  findEta();
  return etaVector;
}

void LeptonJet::findPt() const
{
  for (auto particle : leptonJetParticles)
  {
    double particlePt = particle.pt();
    // ptVector.push_back(particlePt);        
  }
}

void LeptonJet::findPhi() const
{
  for (auto particle : leptonJetParticles)
  {
    double particlePhi = particle.phi();
    // phiVector.push_back(particlePhi);        
  }
}

void LeptonJet::findEta() const
{
  for (auto particle : leptonJetParticles)
  {
    double particleEta = particle.eta();
    // etaVector.push_back(particleEta);        
  }
}


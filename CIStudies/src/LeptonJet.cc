#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

LeptonJet::LeptonJet(const Particle particle)
{
  addParticle(particle);
}

reco::Candidate::LorentzVector LeptonJet::getFourVector() const
{
  reco::Candidate::LorentzVector vector;
  for (auto particle : leptonJetParticles)
  {
    vector += particle.fourVector();
  }
  return vector; // Does this have to be ROOT::Math::LorentzVector?
}

bool LeptonJet::operator==(LeptonJet userJet) const
{
  std::vector<Particle> userParticles = userJet.getParticles();
  return leptonJetParticles == userParticles;
}
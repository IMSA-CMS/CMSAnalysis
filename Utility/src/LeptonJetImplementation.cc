#include "CMSAnalysis/Utility/interface/LeptonJetImplementation.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"


LeptonJetImplementation::LeptonJetImplementation()
{
  //std::cout << "Got to LJI\n";
}
LeptonJetImplementation::LeptonJetImplementation(const LeptonJet* iparticle){
}

reco::Candidate::LorentzVector LeptonJetImplementation::getFourVector() const
{
  reco::Candidate::LorentzVector vector;
  for (auto particle : leptonJetParticles)
  {
    vector += particle.getFourVector();
    // std::cout << "Lepton Jet:" << leptonJetParticles.size() << "\n";
    // std::cout << particle.getPhi() << "\n";
  }

  // std::cout << "Total Phi: " << vector.Phi() << "\n";

  return vector; // Does this have to be ROOT::Math::LorentzVector?
}

bool LeptonJetImplementation::operator==(const ParticleImplementation& userParticle) const
{
    std::cout << "operator\n";
    try
    {
      auto candidateImp = dynamic_cast<const LeptonJetImplementation&>(userParticle);
      std::cout << "operator try\n";
      return candidateImp.getFourVector() == getFourVector() && candidateImp.getParticles() == getParticles();
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}

const ParticleType& LeptonJetImplementation::getType() const
{
  std::cout << "LJI get type\n";
  return ParticleType::leptonJet();

}
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"


LeptonJetImplementation::LeptonJetImplementation(std::shared_ptr<LeptonJet> iLJet) :
  lJet(iLJet)
{
  //std::cout << "Got to LJI\n";
}


reco::Candidate::LorentzVector LeptonJetImplementation::getFourVector() const
{
  return lJet->getFourVector(); // Does this have to be ROOT::Math::LorentzVector?
}

bool LeptonJetImplementation::operator==(const ParticleImplementation& userParticle) const
{
    std::cout << "operator\n";
    try
    {
      auto candidateImp = dynamic_cast<const LeptonJetImplementation&>(userParticle);
      std::cout << "operator try\n";
      return candidateImp.lJet == lJet;
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}

Particle::Type LeptonJetImplementation::getType() const
{
  std::cout << "LJI get type\n";
  return Particle::Type::LeptonJet;

}
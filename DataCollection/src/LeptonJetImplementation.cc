#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"


LeptonJetImplementation::LeptonJetImplementation(std::shared_ptr<LeptonJet> iLJet) :
  lJet(iLJet)
{
  // std::cout << "Got to LJI\n";
}


reco::Candidate::LorentzVector LeptonJetImplementation::getFourVector() const
{
  return lJet->getFourVector(); // Does this have to be ROOT::Math::LorentzVector?
}

bool LeptonJetImplementation::operator==(const ParticleImplementation& userParticle) const
{
    try
    {
      auto candidateImp = dynamic_cast<const LeptonJetImplementation&>(userParticle);
      return candidateImp.lJet == lJet;
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}
double LeptonJetImplementation::energy() const
{
  return lJet->getFourVector().E();

}

Particle::Type LeptonJetImplementation::getType() const
{

  return Particle::Type::LeptonJet;

}

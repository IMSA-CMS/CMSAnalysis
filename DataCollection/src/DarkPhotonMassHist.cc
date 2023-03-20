#include "CMSAnalysis/DataCollection/interface/DarkPhotonMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include <iostream>
#include <math.h>

DarkPhotonMassHist::DarkPhotonMassHist(const std::string& iname, int iNBins, double iminimum, double imaximum):
 HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
{

}

std::vector<double> DarkPhotonMassHist::value() const
{
  //all in GeV
  double muonMass = 0.1057;
  double darkFermionMass = 2;
  double darkPseudoScalarMass = 50;
  double higgsMass = 1000;
  
  auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton()); 

  std::vector<double> gammaVector{};
  std::vector<double> massVector{};
  
  for (auto iparticle : particles.getParticles()) 
  { 
    GenSimParticle particle(iparticle);

    if (particle.finalDaughter() != particle)
    {
      std::cout << "++";
      continue;
    }

    std::vector<Particle> leptons;
    //looping through lepton decays (should be only 2) and pushing to lepton list
    
    for (int j = 0; j < particle.numberOfDaughters(); ++j)
    {
      auto leptonCandidate = particle.daughter(j);
      if (leptonCandidate.getType() == ParticleType::muon())
      {
        leptons.push_back(leptonCandidate);
      }
    }

    if (leptons.size() >= 2)
    {
      auto particle1FourVector = leptons[0].getFourVector();
      auto particle2FourVector = leptons[1].getFourVector();
      auto darkPhotonFourVector = particle1FourVector + particle2FourVector;
      double gamma = darkPhotonFourVector.Gamma();
      gammaVector.push_back(gamma);
    }
  }
  
  for (auto gamma : gammaVector)
  {
    double DarkPhotonMass =  -((((gamma)*(8*muonMass*std::pow(darkFermionMass, 2)))/higgsMass) - std::pow(darkPseudoScalarMass, 2) + std::pow(darkFermionMass, 2));
    
    massVector.push_back(DarkPhotonMass);
  }

  return massVector;
} 
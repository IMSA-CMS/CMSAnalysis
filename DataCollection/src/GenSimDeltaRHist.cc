#include "CMSAnalysis/DataCollection/interface/GenSimDeltaRHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
  

 GenSimDeltaRHist::GenSimDeltaRHist(const std::string& iname, int iNBins, double iminimum, double imaximum):
 HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
 {

 }

  std::vector<double> GenSimDeltaRHist::value() const
{

  //call getparticles or getInput getparticles in input module level-gensim, particle type called darkphoton in particle class
  //particles = GetInput() from input module?
  //not in particle.cc, just pdgid == 4900022 for identifying dark photons
  auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton());

  //loop through number of events to get all final daughters
  //finalDaughter(0...1...2...)
  // for (auto particle : particles.getParticles())
  // {
  //   while (particle != particle.finalDaughter())
  //   {
  //     particle = particle.finalDaughter();
  //   }
  // }

  // if (particle.getType() == 4900022)
  // {
  // }

  std::vector<double> deltaRVector{};

  for (GenSimParticle particle : particles.getParticles()) 
  {
    // if (particle.getType() == Particle::Type::DarkPhoton)
    // {
    // }

    //std::cout << particle.finalDaughter().getType();

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

    //get deltaR between the 2 muons in this lepton jet and pushes deltaR to return vector
    if (leptons.size() >= 2)
    {
      auto particle1FourVector = leptons[0].getFourVector();
      auto particle2FourVector = leptons[1].getFourVector();
      double deltaR = reco::deltaR(particle1FourVector, particle2FourVector);
      deltaRVector.push_back(deltaR);
    }
  }
  return deltaRVector;
}
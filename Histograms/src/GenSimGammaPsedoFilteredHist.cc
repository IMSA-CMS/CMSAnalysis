#include "CMSAnalysis/Histograms/interface/GenSimGammaPsedoFilteredHist.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include "CMSAnalysis/Utility/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"


#include <iostream>

GenSimGammaPsedoFilteredHist::GenSimGammaPsedoFilteredHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetReconstructionModule> lepJetModule):
 HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
 leptonJets(lepJetModule)

{ 
}

std::vector<double> GenSimGammaPsedoFilteredHist::value() const
{
  //call getparticles or getInput getparticles in input module level-gensim, particle type called darkphoton in particle class
  //particles = GetInput() from input module?
  //not in particle.cc, just pdgid == 4900022 for identifying dark photons
  auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton());

  std::vector<double> gammaVector{};

  for (auto iparticle : particles.getParticles())  
  { 
    GenSimParticle particle(iparticle);
    // std::cout << "Particle/n ";
    // if (particle.getType() == Particle::Type::DarkPhoton)
    // {
    // }

    if (particle.finalDaughter() != particle)
    {
      std::cout << "++";
      continue;
    }

    std::vector<Particle> leptons;
    //looping through lepton decays (should be only 2) and pushing to lepton list
    //std::cout<<"ParticleType: " << particle.getName() << "\n";
    //std::cout<<"Daughters: " << particle.numberOfDaughters() << "\n";

    for (int j = 0; j < particle.numberOfDaughters(); ++j)
    {
      auto leptonCandidate = particle.daughter(j);
      if (leptonCandidate.getType() == ParticleType::muon())
      {
        leptons.push_back(leptonCandidate);
      }
    }

  std::vector<LeptonJet> leptonJetsVector = leptonJets->getLeptonJets();
  int numberOfLeptonJets = leptonJetsVector.size();
  bool returnBool; 
  if(numberOfLeptonJets == 0) {
    returnBool = false;
  }
  else if(numberOfLeptonJets == 1){
    returnBool = true;

  }
  else {
    returnBool = false;
  }

    if (leptons.size() >= 2 && returnBool)
    {
      auto particle1FourVector = leptons[0].getFourVector();
      auto particle2FourVector = leptons[1].getFourVector();
      auto darkPhotonFourVector = particle1FourVector + particle2FourVector;
      double gamma = darkPhotonFourVector.Gamma();
      gammaVector.push_back(gamma);
    }
  }
  return gammaVector;
}
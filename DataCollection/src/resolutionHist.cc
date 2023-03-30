#include "CMSAnalysis/DataCollection/interface/resolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include <iostream>

resolutionHist::resolutionHist(std::shared_ptr<LeptonJetMatchingModule> LeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum, std::string ianalysis):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    LeptonJetMatchingModule(iLeptonJetMatchingModule),
    analysis(ianalysis)
{

}

std::vector<double> resolutionHist::value() const
{
  auto matchedPairs = LeptonJetMatchingModule.getMatchingPairs()
  for(pairs : matchedPairs) {
    pairs.get(first)



  }
  std::vector<double> gammaVector{};
  std::vector<double> deltaRVector{};
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
  
  return gammaVector;
}

std::vector<double> resolutionHist::recoGamma(std::vector<LeptonJet> LeptonJets)
  {
    std::vector<double> gamma_leptonJets; 

    if (LeptonJets.size() > 0)
    {
        for (uint i = 0; i < LeptonJets.size(); i++)
       {
         gamma_leptonJets.push_back(LeptonJets[i].getGamma());
       }
        return {gamma_leptonJets};
    }
    else
    {
    return {-1};
    }  
  } 

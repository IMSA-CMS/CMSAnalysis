 #include "CMSAnalysis/DataCollection/interface/resolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include <iostream>

resolutionHist::resolutionHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum, std::string ianalysis):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonJetRecoModule(iLeptonJetRecoModule),
    analysis(ianalysis)
{

}

std::vector<double> resolutionHist::value() const
{
  //Gen Sim section
  auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton());
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
    auto particle1FourVector = leptons[0].getFourVector();
    auto particle2FourVector = leptons[1].getFourVector();
    if (analysis=="Gamma")
    {
        if (leptons.size() >= 2)
        {
            
            auto darkPhotonFourVector = particle1FourVector + particle2FourVector;
            double gamma = darkPhotonFourVector.Gamma();
            gammaVector.push_back(gamma);
        }
    }
    else if (analysis=="Delta R")
    {
        double deltaR = reco::deltaR(particle1FourVector, particle2FourVector);
        deltaRVector.push_back(deltaR);
    }
    else 
    {
        std::runtime_error("analysis incorectly inputed");
    }
  }
  
  return gammaVector;
} 
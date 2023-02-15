#include "CMSAnalysis/DataCollection/interface/GammaDeltaRHist2D.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

#include <vector>

GammaDeltaRHist2D::GammaDeltaRHist2D(std::shared_ptr<LeptonJetReconstructionModule> ileptonJetRecoModule, const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY),
    leptonJetRecoModule(ileptonJetRecoModule)
    {} 

std::vector<std::pair<double, double>> GammaDeltaRHist2D::value2D() const

{
  //std::cout<<"entered file" << "\n"; 

  //std::vector<std::pair<double, double>> gammaDeltaRvalues;

  //std::vector<double> gamma_leptonJets; 

  //new
  //std::cout << typeid(leptonJetRecoModule).name() << '\n';
  //std::cout << typeid(*leptonJetRecoModule).name();
  auto LeptonJets = leptonJetRecoModule->getLeptonJets();
  
  //std::cout<< "got leptons" << "\n";

  std::vector<std::pair<double, double>> gammaDeltaRpairs;

  uint numJets = LeptonJets.size();

  //std::cerr<< "got num jets" << "\n";

  //for (LeptonJet jet : LeptonJets)
  for (uint i = 0; i < numJets; i++)
  {
    //std::cerr << "in for loop\n";
    const std::vector<Particle>& particles = LeptonJets[i].getParticles();

    //std::cout<< "particles gamma: " << LeptonJets[i].getGamma() << "\n" << "particles Delta R: " << particles[0].getDeltaR(particles[1]);

    gammaDeltaRpairs.push_back({{LeptonJets[i].getGamma()}, {particles[0].getDeltaR(particles[1])}});


    /*for (Particle p : jet.getParticles())
	  {
		  for (Particle q : jet.getParticles())
		  {
			  //if (p.getDeltaR(q) > deltaR)
			  //{
				double deltaR = p.getDeltaR(q);

                std::cout << jet.getGamma();
                std::cout << deltaR;
                gammaDeltaRpairs.push_back({{jet.getGamma()}, {deltaR}});

			  //}
		  }
	  }*/
  }
  //std::cout << "reached end" << "\n";
  return gammaDeltaRpairs;
  
  /*if (LeptonJets.size() > 0)
  {
      for (uint i = 0; i < LeptonJets.size(); i++)
      {
        gamma_leptonJets.push_back(LeptonJets[i].getGamma());
      }
  }
  else    
  {
      return {};
  } 

  const std::vector<double>& deltaRValues = leptonJetRecoModule->getDeltaRValues();

  if (deltaRValues.size() > gamma_leptonJets.size())
  {
      for (uint j = 0; j < gamma_leptonJets.size(); j++)
      {
        gammaDeltaRvalues.push_back({{gamma_leptonJets[j]}, {deltaRValues[j]}});
      }
  }
  else
  {
      for (uint j = 0; j < deltaRValues.size(); j++)
      {
        gammaDeltaRvalues.push_back({{gamma_leptonJets[j]}, {deltaRValues[j]}});
      }
  }
  return gammaDeltaRvalues;*/
};
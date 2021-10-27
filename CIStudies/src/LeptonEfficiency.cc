//#define _GLIBCXX_USE_CXX11_ABI 0
//#include "TROOT.h"
//#include "TSystem.h"

#include <iostream>

//#include "CIAnalysis/CIStudies/interface/TDisplayText.h"

#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"

LeptonEfficiency::LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  matchModule(imatchModule)
  //,genSimModule(igenSimModule)
  ,recoMuons(0)
  ,genSimMuons(0)
  ,recoElectrons(0)
  ,genSimElectrons(0)
{
}

bool LeptonEfficiency::process()
{
  auto genSim = getInput()->getParticles(InputModule::RecoLevel::GenSim);

  // std::cout << "Looking at GenSim Particles" << std::endl;
  for(const auto &particle : genSim.getParticles())
    {
      auto type = particle.getType();
      if(type == Particle::Type::Electron)
	{
	  genSimElectrons++;
	  // std::cout << "Electron" << std::endl;
	}
      else if(type == Particle::Type::Muon)
	{
	  genSimMuons++;
	  // std::cout << "Muon" << std::endl;
	}
    }

  const MatchingPairCollection& matched = matchModule->getMatchingBestPairs();

  // std::cout << "Looking at Reco Particles" << std::endl;
  auto particles = matched.getRecoParticles().getParticles();
  for(const auto &particle : particles)
    {
      if(particle.isNotNull())
	{
	  auto type = particle.getType();

	  if(type == Particle::Type::Electron)
	    {
	      recoElectrons++;
	      // std::cout << "Reco Electron" << std::endl;
	    }
	  else if(type == Particle::Type::Muon)
	    {
	      recoMuons++;
	      // std::cout << "Reco Muon" << std::endl;
	    }
	}
      else
	{
	  //std::cout << "Null Particle at line " << __LINE__ << std::endl;
	}
    }
  return true;
}
void LeptonEfficiency::finalize()
{
  std::string muonOutputString = std::to_string(recoMuons/(double)genSimMuons);
  writeText(muonOutputString, "Muon Efficiency");

  std::string electronOutputString = std::to_string(recoElectrons/(double)genSimElectrons);
  writeText(electronOutputString, "Electron Efficiency");
}

#include <iostream>

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonEfficiency.hh"

LeptonEfficiency::LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
  ,recoMuons(0)
  ,genSimMuons(0)
  ,recoElectrons(0)
  ,genSimElectrons(0)
{
}

bool LeptonEfficiency::process()
{
  auto genSim = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
  if(getInput()){
    std::cout << "Pointer is full\n";
  }
  

  for(const auto &particle : genSim.getParticles())
  {
    std::cout << "LeptonEfficiency22";

    auto type = particle.getType();
    if(type == ParticleType::electron())
	  {
	    genSimElectrons++;
      std::cout << "LeptonEfficiency28";

	  }
    else if(type == ParticleType::muon())
	  {
	    genSimMuons++;
      std::cout << "LeptonEfficiency34";
	  }
  }

  const MatchingPairCollection& matched = matchModule->getMatchingBestPairs();

  auto particles = matched.getRecoParticles().getParticles();
  for(const auto &particle : particles)
  {
    if(particle.isNotNull())
    {
      auto type = particle.getType();
      if(type == ParticleType::electron())
      {
        recoElectrons++;
      }
    else if(type == ParticleType::muon())
      {
        recoMuons++;
      }
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

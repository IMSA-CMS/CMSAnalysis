#include <iostream>

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusEfficiency.hh"

HPlusPlusEfficiency::HPlusPlusEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
  ,genSimMuons(0)
  ,genSimElectrons(0)
  ,genSimHPlusPlus(0)
{
}

bool LeptonEfficiency::process()
{
  auto genSim = getInput()->getLeptons(InputModule::RecoLevel::GenSim);

  

  for(const auto &particle : genSim.getParticles())
  {

    auto type = particle.getType();
    if(type == ParticleType::electron())
	  {
	    genSimElectrons++;

	  }
    else if(type == ParticleType::muon())
	  {
	    genSimMuons++;
	  }
    else if(type == ParticleType::leftDoublyHiggs() || type == ParticleType::rightDoublyHiggs())
      {
        genSimHPlusPlus++;
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
        recoElectronsList.add(particle);
      }
    else if(type == ParticleType::muon())
      {
        recoMuonsList.add(particle);
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
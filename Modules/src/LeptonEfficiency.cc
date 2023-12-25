#include <iostream>

//#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"

#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonEfficiency.hh"

LeptonEfficiency::LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
{
  addRequiredModule(imatchModule);
}

bool LeptonEfficiency::process()
{
  auto genSim = getInput()->getLeptons(EventInput::RecoLevel::GenSim);

  incrementCounter("genSimElectron", 0);
  incrementCounter("genSimMuon", 0);
  incrementCounter("recoElectron", 0);
  incrementCounter("recoMuon", 0);

  for(const auto &particle : genSim.getParticles())
  {

    auto type = particle.getType();
    if(type == ParticleType::electron())
	  {
	    incrementCounter("genSimElectron", 1);

	  }
    else if(type == ParticleType::muon())
	  {
	    incrementCounter("genSimMuon", 1);
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
        incrementCounter("recoElectron", 1);
      }
    else if(type == ParticleType::muon())
      {
        incrementCounter("recoMuon", 1);
      }
    }
  }
  return true;
}

void LeptonEfficiency::finalize()
{
  std::cout << "beginning\n";
  std::string muonEndcapOutputString = std::to_string(getCounter("recoMuon")/(double)getCounter("genSimMuon"));
  writeText(muonEndcapOutputString, "Muon Efficiency");
  std::cout << "middle\n";
  std::string electronBarrelOutputString = std::to_string(getCounter("recoElectron")/(double)getCounter("genSimElectron"));
  writeText(electronBarrelOutputString, "Electron Efficiency");
  std::cout << "end\n";
}

<<<<<<< HEAD:DataCollection/src/EfficiencyModule.cc
#include <iostream>
=======
#include "CMSAnalysis/Modules/interface/EfficiencyModule.hh"
>>>>>>> e2e8b8bc66570119d7ab2e705350da70f9b651d1:Modules/src/EfficiencyModule.cc

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusEfficiency.hh"

<<<<<<< HEAD:DataCollection/src/EfficiencyModule.cc
HPlusPlusEfficiency::HPlusPlusEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
  ,genSimMuons(0)
  ,genSimElectrons(0)
  ,genSimHPlusPlus(0)
=======
bool EfficiencyModule::process ()
>>>>>>> e2e8b8bc66570119d7ab2e705350da70f9b651d1:Modules/src/EfficiencyModule.cc
{
}

bool LeptonEfficiency::process()
{
<<<<<<< HEAD:DataCollection/src/EfficiencyModule.cc
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
=======
    // if (increment != 0)
    // {
    //     std::cout << name << "+" << std::to_string(increment) << std::endl;
    // }
    if(counters.find(name) == counters.end())
>>>>>>> e2e8b8bc66570119d7ab2e705350da70f9b651d1:Modules/src/EfficiencyModule.cc
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

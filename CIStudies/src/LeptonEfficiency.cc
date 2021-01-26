#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"

LeptonEfficiency::LeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule, const std::shared_ptr<GenSimIdentificationModule> igenSimModule):
  matchModule(imatchModule)
  ,genSimModule(igenSimModule)
  ,recoMuons(0)
  ,genSimMuons(0)
  ,recoElectrons(0)
  ,genSimElectrons(0)
{
}

bool LeptonEfficiency::process(const edm::EventBase& event)
{
  auto genSim = genSimModule->getGenParticles();

  // std::cout << "Looking at GenSim Particles" << std::endl;
  for(const auto &particle : genSim.getParticles())
    {
      auto type = particle.getLeptonType();
      if(type == Particle::LeptonType::Electron)
	{
	  genSimElectrons++;
	  // std::cout << "Electron" << std::endl;
	}
      else if(type == Particle::LeptonType::Muon)
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
	  auto type = particle.getLeptonType();

	  if(type == Particle::LeptonType::Electron)
	    {
	      recoElectrons++;
	      // std::cout << "Reco Electron" << std::endl;
	    }
	  else if(type == Particle::LeptonType::Muon)
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
  std::cout << "Muon efficiency: " << recoMuons / (double) genSimMuons << std::endl;
  std::cout << "Electron efficiency: " << recoElectrons / (double) genSimElectrons << std::endl;
}

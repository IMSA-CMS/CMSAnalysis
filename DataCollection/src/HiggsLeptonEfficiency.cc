#include <iostream>

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsLeptonEfficiency.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"

HiggsLeptonEfficiency::HiggsLeptonEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
{
}

bool HiggsLeptonEfficiency::process()
{
  auto genSim = getInput()->getLeptons(InputModule::RecoLevel::GenSim);

  for(const auto &particle : genSim.getParticles())
  {
    auto genSimPart = GenSimParticle(particle);
    auto type = particle.getType();

    if ((genSimPart.uniqueMother().getType() != ParticleType::leftDoublyHiggs() && genSimPart.uniqueMother().getType() != ParticleType::rightDoublyHiggs()) || genSimPart != genSimPart.finalDaughter())
    {
      continue;
    }

    if(type == ParticleType::electron())
	  {
      incrementCounter("genSimLepton", 1);
      if (particle.getBarrelState() == Particle::BarrelState::Barrel)
      {
        incrementCounter("genSimBarrelElectron", 1);
      } else if (particle.getBarrelState() == Particle::BarrelState::Endcap)
      {
        incrementCounter("genSimEndcapElectron", 1);
      }
	  }
    else if(type == ParticleType::muon())
	  {
      incrementCounter("genSimLepton", 1);
	    if (particle.getBarrelState() == Particle::BarrelState::Barrel)
      {
        incrementCounter("genSimBarrelMuon", 1);
      } else if (particle.getBarrelState() == Particle::BarrelState::Endcap)
      {
        incrementCounter("genSimEndcapMuon", 1);
      }
	  }
  }
  const MatchingPairCollection& matched = matchModule->getMatchingBestPairs();
  for(const auto &match : matched.getPairs())
  {
    auto particle = match.getRecoParticle();
    auto genParticle = match.getGenParticle();
    auto type = particle.getType();
    if(particle.isNotNull())
    {

      try 
      { //if statement checks type to avoid double counting a lepton if it is mismatched to a lepton of another type.
        if (type != genParticle.getType() || (genParticle.uniqueMother().getType() != ParticleType::leftDoublyHiggs() && genParticle.uniqueMother().getType() != ParticleType::rightDoublyHiggs()))
        {
          continue;
        }
      } catch (std::runtime_error& e) //Try catch statement handles case when genParticle's mother is null. In this case it is safe to assume that we don't the particle anyway.
      {
        continue;
      }

      if(type == ParticleType::electron())
      {
        incrementCounter("recoLepton", 1);
        if (particle.getBarrelState() == Particle::BarrelState::Barrel)
        {
          incrementCounter("recoBarrelElectron", 1);
        } else if (particle.getBarrelState() == Particle::BarrelState::Endcap)
        {
          incrementCounter("recoEndcapElectron", 1);
        }
      }
      else if(type == ParticleType::muon())
      {
        incrementCounter("recoLepton", 1);
        if (particle.getBarrelState() == Particle::BarrelState::Barrel)
        {
          incrementCounter("recoBarrelMuon", 1);
        } else if (particle.getBarrelState() == Particle::BarrelState::Endcap)
        {
          incrementCounter("recoEndcapMuon", 1);
        }
      }
    }
  }
  return true;
}

void HiggsLeptonEfficiency::finalize()
{
  std::string muonBarrelOutputString = std::to_string(getCounter("recoBarrelMuon")/(double)getCounter("genSimBarrelMuon"));
  writeText(muonBarrelOutputString, "Muon Barrel Efficiency");

  std::string muonEndcapOutputString = std::to_string(getCounter("recoEndcapMuon")/(double)getCounter("genSimEndcapMuon"));
  writeText(muonEndcapOutputString, "Muon Endcap Efficiency");

  std::string electronBarrelOutputString = std::to_string(getCounter("recoBarrelElectron")/(double)getCounter("genSimBarrelElectron"));
  writeText(electronBarrelOutputString, "Electron Barrel Efficiency");

  std::string electronEndcapOutputString = std::to_string(getCounter("recoEndcapElectron")/(double)getCounter("genSimEndcapElectron"));
  writeText(electronEndcapOutputString, "Electron Endcap Efficiency");

  std::string leptonOutputString = std::to_string(getCounter("recoLepton")/(double)getCounter("genSimLepton"));
  writeText(leptonOutputString, "Lepton Efficiency");

  std::string leptonOutputString2 = std::to_string(getCounter("genSimLepton"));
  writeText(leptonOutputString2, "Total Leptons");
}

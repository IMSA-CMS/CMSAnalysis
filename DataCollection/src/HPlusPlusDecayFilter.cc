#include "CMSAnalysis/DataCollection/interface/HPlusPlusDecayFilter.hh"

HPlusPlusDecayFilter::HPlusPlusDecayFilter(InputModule::RecoLevel isGenSim):
typeGenSim(isGenSim)
{};

std::string HPlusPlusDecayFilter::makeFilterString()
{
  int higgsPlus = 0;
  int higgsMinus = 0;
  if (typeGenSim == InputModule::RecoLevel::GenSim)
  {
    auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    for (const auto &particle : particles) //cycles through to find the doubly charged higgs
    {
      GenSimParticle genSimParticle = GenSimParticle(particle);
      if ((genSimParticle.pdgId() == 9900041 || genSimParticle.pdgId() == 9900042) && 
      genSimParticle == genSimParticle.finalDaughter() && genSimParticle.numberOfDaughters() == 2) 
      {
        higgsPlus = std::abs(genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId()); //adds the pdgid of the particles for easier processing
      } else if ((genSimParticle.pdgId() == -9900041 || genSimParticle.pdgId() == -9900042) && 
      genSimParticle == genSimParticle.finalDaughter() && genSimParticle.numberOfDaughters() == 2) // H--
      {
        higgsMinus = std::abs(genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId());
      }
      if (higgsMinus != 0 && higgsPlus != 0) //exits once both are found
      {
        break;
      }
    }
  } else if (typeGenSim == InputModule::RecoLevel::Reco)
  {
    auto leptons = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    for (const auto &lepton : leptons)
    {
      if (lepton.getCharge() > 0)
      {
        if (lepton.getType() == ParticleType::electron())
        {
          higgsPlus += 11;
        } else if (lepton.getType() == ParticleType::muon())
        {
          higgsPlus += 13;
        }
      } else if (lepton.getCharge() < 0)
      {
        if (lepton.getType() == ParticleType::electron())
        {
          higgsMinus += 11;
        } else if (lepton.getType() == ParticleType::muon())
        {
          higgsMinus += 13;
        }
      }
    }
  }
  if (higgsPlus == 22) //classifies them based off of pdgID: 22 is ee, 24 is eu, 26 is uu
  {
    if (higgsMinus == 22)
    {
      
      return "eeee";
    } else if (higgsMinus == 24)
    {
      return "eeeu";
    } else if (higgsMinus == 26)
    {
      return "eeuu";
    }
  } else if (higgsPlus == 26)
  {
    if (higgsMinus == 22)
    {
      return "eeuu";
    } else if (higgsMinus == 24)
    {
      return "euuu";
    } else if (higgsMinus == 26)
    {
      return "uuuu";
    }
  } else if (higgsPlus == 24)
  {
    if (higgsMinus == 22)
    {
      return "eeeu";
    } else if (higgsMinus == 24)
    {
      return "eueu";
    } else if (higgsMinus == 26)
    {
      return "euuu";
    }
  }
  return "none";
}

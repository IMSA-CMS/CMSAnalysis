#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"

HPlusPlusDecayFilter::HPlusPlusDecayFilter(EventInput::RecoLevel isGenSim):
typeGenSim(isGenSim)
{};

std::string HPlusPlusDecayFilter::getFilterString(const EventInput* inputMod) const
{
  int higgsPlus = 0;
  int higgsMinus = 0;
  if (typeGenSim == EventInput::RecoLevel::GenSim)
  {
    auto particles = inputMod->getParticles(EventInput::RecoLevel::GenSim);
    for (const auto &particle : particles) //cycles through to find the doubly charged higgs
    {
      GenSimParticle genSimParticle = GenSimParticle(particle);
      if ((genSimParticle.pdgId() == 9900041 || genSimParticle.pdgId() == 9900042) && genSimParticle == genSimParticle.finalDaughter() && genSimParticle.numberOfDaughters() == 2) // H++
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
  } else if (typeGenSim == EventInput::RecoLevel::Reco)
  {
    auto leptons = inputMod->getLeptons(EventInput::RecoLevel::Reco);
    
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

  if (higgsPlus == 22) //classifies them based off of pdgID: 22 is ee, 24 is eu, 26 is uu, 11 is single e, 13 is single u
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
    } else if (higgsMinus == 11)
    {
      return "eee";
    } else if (higgsMinus == 13)
    {
      return "eeu";
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
    } else if (higgsMinus == 11)
    {
      return "uue";
    } else if (higgsMinus == 13)
    {
      return "uuu";
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
    } else if (higgsMinus == 11)
    {
      return "eue";
    } else if (higgsMinus == 13)
    {
      return "euu";
    }
  } else if (higgsPlus == 11)
  {
    if (higgsMinus == 22)
    {
      return "eee";
    } else if (higgsMinus == 24)
    {
      return "eue";
    } else if (higgsMinus == 26)
    {
      return "uue";
    }
  } else if (higgsPlus == 13)
  {
    if (higgsMinus == 22)
    {
      return "eeu";
    } else if (higgsMinus == 24)
    {
      return "euu";
    } else if (higgsMinus == 26)
    {
      return "uuu";
    }
  }
  return "none";
}

std::string HPlusPlusDecayFilter::getState(ParticleCollection<Particle> genSim, ParticleCollection<Particle> reco)
{
  int higgsPlus = 0;
  int higgsMinus = 0;
  if (typeGenSim == EventInput::RecoLevel::GenSim)
  {
    auto particles = genSim;
    for (const auto &particle : particles) //cycles through to find the doubly charged higgs
    {
      GenSimParticle genSimParticle = GenSimParticle(particle);
      // if(true)
      // {
      //   std::cout << particle.getName() << " " << genSimParticle.pdgId() << " " << particle.getCharge() << " " << genSimParticle.numberOfDaughters() << std::endl;
      // }
      if (genSimParticle.numberOfDaughters() == 2 && genSimParticle.daughter(0).getName() != "Tau" && genSimParticle.daughter(1).getName() != "Tau")
      {
        if ((genSimParticle.pdgId() == 19800084) && (particle.getCharge() > 0)) // H++
        {
          higgsPlus = std::abs(genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId());
          //std::cout << genSimParticle.daughter(0).getName() + " " + genSimParticle.daughter(1).getName() << std::endl;
          //std::cout << std::to_string(std::abs(genSimParticle.daughter(0).pdgId())) << "+" << std::to_string(std::abs(genSimParticle.daughter(0).pdgId())) << "=" << std::to_string(higgsPlus) << std::endl;
        } else if ((genSimParticle.pdgId() == 19800084) && (particle.getCharge() < 0)) // H--
        {
          higgsMinus = std::abs(genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId());
          //std::cout << genSimParticle.daughter(0).getName() + " " + genSimParticle.daughter(1).getName() << std::endl;
          //std::cout << std::to_string(std::abs(genSimParticle.daughter(0).pdgId())) << "+" << std::to_string(std::abs(genSimParticle.daughter(0).pdgId())) << "=" << std::to_string(higgsMinus) << std::endl;
        }
      }
      if (higgsMinus != 0 && higgsPlus != 0) //exits once both are found
      {
        break;
      }
    }
  } else if (typeGenSim == EventInput::RecoLevel::Reco)
  {
    auto leptons = reco;
    std::string leps;
    for (const auto &lepton : leptons) //cycles through to find the doubly charged higgs
    {
      leps += lepton.getName() + " ";
      if (lepton.getCharge() > 0)
      {
        if (lepton.getName() == "Electron")
        {
          higgsPlus += 11;
        } else if (lepton.getName() == "Muon")
        {
          higgsPlus += 13;
        }
      } else if (lepton.getCharge() < 0)
      {
        if (lepton.getName() == "Electron")
        {
          higgsMinus += 11;
        } else if (lepton.getName() == "Muon")
        {
          higgsMinus += 13;
        }
      }
    }
    //std::cout << leps << std::to_string(higgsPlus) << " " << std::to_string(higgsMinus) << std::endl;
  }
  if (higgsPlus == 22) //classifies them based off of pdgID: 22 is ee, 24 is eu, 26 is uu, 11 is single e, 13 is single u
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
    } else if (higgsMinus == 11)
    {
      return "eee";
    } else if (higgsMinus == 13)
    {
      return "eeu";
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
    } else if (higgsMinus == 11)
    {
      return "uue";
    } else if (higgsMinus == 13)
    {
      return "uuu";
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
    } else if (higgsMinus == 11)
    {
      return "eue";
    } else if (higgsMinus == 13)
    {
      return "euu";
    }
  } else if (higgsPlus == 11)
  {
    if (higgsMinus == 22)
    {
      return "eee";
    } else if (higgsMinus == 24)
    {
      return "eue";
    } else if (higgsMinus == 26)
    {
      return "uue";
    }
  } else if (higgsPlus == 13)
  {
    if (higgsMinus == 22)
    {
      return "eeu";
    } else if (higgsMinus == 24)
    {
      return "euu";
    } else if (higgsMinus == 26)
    {
      return "uuu";
    }
  }
  return "none";
}
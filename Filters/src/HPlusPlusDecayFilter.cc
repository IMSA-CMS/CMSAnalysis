#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"


HPlusPlusDecayFilter::HPlusPlusDecayFilter(EventInput::RecoLevel isGenSim):
typeGenSim(isGenSim)
{};


int HPlusPlusDecayFilter::getIndex(std::string* arr, std::string elem, int size)
{
  for (int i = 0; i < size; i++)
  {
    if (arr[i].compare(elem) == 0)
    {
      return i;
    }
  }
  return -1;
}


std::string HPlusPlusDecayFilter::getFilterString(const EventInput* inputMod) const
{
  return getState(inputMod);
}


std::string HPlusPlusDecayFilter::getState(const EventInput* inputMod) const
{
  auto genSim = inputMod->getParticles(EventInput::RecoLevel::GenSim);
  auto reco = inputMod->getLeptons(EventInput::RecoLevel::Reco);


  if (typeGenSim == EventInput::RecoLevel::GenSim)
  {
    std::string hPlus = "";
    std::string hMinus = "";


    std::string c1str[] = {"e", "u", "t"};
    std::string c2str[] = {"ee", "eu", "et", "uu", "ut", "tt"};

    for (const auto &particle : genSim) //cycles through to find the doubly charged higgs
    {
      GenSimParticle genSimParticle = GenSimParticle(particle);

      if (inputMod->getParticles(EventInput::RecoLevel::GenSim, ParticleType::electron()).size() < 2)
      {
        // std::cout << particle.getName() << " (" << genSimParticle.pdgId() << "): " << std::to_string(particle.getCharge()) << std::endl;
      }

      if ((genSimParticle.pdgId() == 9900041 || genSimParticle.pdgId() == 9900042) && genSimParticle == genSimParticle.finalDaughter() && genSimParticle.numberOfDaughters() == 2) // H++
      {
        int p1i = (std::abs((genSimParticle.daughter(0).pdgId())) - 11) / 2;
        int p2i = (std::abs((genSimParticle.daughter(1).pdgId())) - 11) / 2;

        std::string p1 = c1str[p1i];
        std::string p2 = c1str[p2i];

        if (p1i <= p2i)
        {
          hPlus = p1 + p2;
        }
        else
        {
          hPlus = p2 + p1;
        }
      }
      else if ((genSimParticle.pdgId() == -9900041 || genSimParticle.pdgId() == -9900042) && genSimParticle == genSimParticle.finalDaughter() && genSimParticle.numberOfDaughters() == 2) // H--
      {
        int p1i = (std::abs((genSimParticle.daughter(0).pdgId())) - 11) / 2;
        int p2i = (std::abs((genSimParticle.daughter(1).pdgId())) - 11) / 2;

        std::string p1 = c1str[p1i];
        std::string p2 = c1str[p2i];

        if (p1i <= p2i)
        {
          hMinus = p1 + p2;
        }
        else
        {
          hMinus = p2 + p1;
        }
      }
      // std::cout << hPlus << std::endl;
      // std::cout << hMinus << std::endl;
      if (hMinus != "" && hPlus != "") //exits once both are found
      {
        break;
      }
    }

    if (hMinus == "" && hPlus == "")
    {
      return "none";
    }

    if (getIndex(c2str, hPlus, 6) <= getIndex(c2str, hMinus, 6))
    {
      // std::cout << hPlus << "," << hMinus << std::endl;
      return hPlus + hMinus;
    }
    else
    {
      // std::cout << hMinus << "," << hPlus << std::endl;
      return hMinus + hPlus;
    }
  }
  else if (typeGenSim == EventInput::RecoLevel::Reco)
  {
    int higgsPlus = 0;
    int higgsMinus = 0;

    int leptons = 0;

    for (const auto &lepton : reco)
    {
      if (inputMod->getParticles(EventInput::RecoLevel::Reco, ParticleType::electron()).size() < 2)
      {
        // std::cout << lepton.getName() << ": " << std::to_string(lepton.getCharge()) << std::endl;
      }

      //std::cout << "GENSIM/RECO " << lepton.getName() << " (" << genSimParticle.pdgId() << "): " << std::to_string(lepton.getCharge()) << std::endl;
      //std::cout << "DAUGHTER " << genSimParticle.finalDaughter().getName() << " (" << genSimParticle.finalDaughter().pdgId() << "): " << std::to_string(genSimParticle.finalDaughter().getCharge()) << std::endl;

      if (lepton.getCharge() > 0)
      {
        if (lepton.getType() == ParticleType::electron())
        {
          higgsPlus += 11;
          leptons++;
        } else if (lepton.getType() == ParticleType::muon())
        {
          higgsPlus += 13;
          leptons++;
        }
      } else if (lepton.getCharge() < 0)
      {
        if (lepton.getType() == ParticleType::electron())
        {
          higgsMinus += 11;
          leptons++;
        } else if (lepton.getType() == ParticleType::muon())
        {
          higgsMinus += 13;
          leptons++;
        }
      }
    }

    //std::cout << std::to_string(higgsPlus) << " " << std::to_string(higgsMinus) << std::endl;

    if (higgsPlus > 26 || higgsMinus > 26)
    {
      return "none";
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
      } else if (higgsMinus == 0)
      {
        return "ee";
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
      } else if (higgsMinus == 0)
      {
        return "uu";
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
      } else if (higgsMinus == 0)
      {
        return "eu";
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
      } else if (higgsMinus == 11)
      {
        return "e e";
      } else if (higgsMinus == 13)
      {
        return "e u";
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
      } else if (higgsMinus == 11)
      {
        return "e u";
      } else if (higgsMinus == 13)
      {
        return "u u";
      }
    } else if (higgsPlus == 0)
    {
      if (higgsMinus == 22)
      {
        return "ee";
      } else if (higgsMinus == 24)
      {
        return "eu";
      } else if (higgsMinus == 26)
      {
        return "uu";
      }
    }
  }
  return "none";
}

#include "CMSAnalysis/DataCollection/interface/HPlusPlusDecayFilter.hh"

HPlusPlusDecayFilter::HPlusPlusDecayFilter(){};

std::string HPlusPlusDecayFilter::makeFilterString()
{
  auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
  int higgsPlus = 0;
  int higgsMinus = 0;
  for (const auto &particle : particles) //cycles through to find the doubly charged higgs
  {
    GenSimParticle genSimParticle = GenSimParticle(particle);
    if ((genSimParticle.pdgId() == 9900041 || genSimParticle.pdgId() == 9900042) && genSimParticle == genSimParticle.finalDaughter()) // H++
    {
      if (genSimParticle.numberOfDaughters() == 2 &&
        (genSimParticle.daughter(0).pdgId() == 13 || genSimParticle.daughter(0).pdgId() == 11) &&
        (genSimParticle.daughter(1).pdgId() == 13 || genSimParticle.daughter(1).pdgId() == 11))
      {
        higgsPlus = genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId();
      }
    } else if ((genSimParticle.pdgId() == -9900041 || genSimParticle.pdgId() == -9900042) && genSimParticle == genSimParticle.finalDaughter()) // H++
    {
      if (genSimParticle.numberOfDaughters() == 2 &&
        (genSimParticle.daughter(0).pdgId() == -13 || genSimParticle.daughter(0).pdgId() == -11) &&
        (genSimParticle.daughter(1).pdgId() == -13 || genSimParticle.daughter(1).pdgId() == -11))
      {
        higgsMinus = genSimParticle.daughter(0).pdgId() + genSimParticle.daughter(1).pdgId();
      }
    }
    if (higgsMinus != 0 && higgsPlus != 0) //exits once both are found
    {
      break;
    }
  }
  if (higgsPlus == 22) //classifies them based off of pdgID: 22 is ee, 24 is eu, 26 is uu
  {
    if (higgsMinus == -22)
    {
      
      return "eeee";
    } else if (higgsMinus == -24)
    {
      return "eeeu";
    } else if (higgsMinus == -26)
    {
      return "eeuu";
    }
  } else if (higgsPlus == 26)
  {
    if (higgsMinus == -22)
    {
      return "uuee";
    } else if (higgsMinus == -24)
    {
      return "uueu";
    } else if (higgsMinus == -26)
    {
      return "uuuu";
    }
  } else if (higgsPlus == 24)
  {
    if (higgsMinus == -22)
    {
      return "euee";
    } else if (higgsMinus == -24)
    {
      return "eueu";
    } else if (higgsMinus == -26)
    {
      return "euuu";
    }
  }
  return "none";
}

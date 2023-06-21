#include "CMSAnalysis/DataCollection/interface/HPlusPlusEfficiency.hh"
#include <iostream>

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusGenSimSelector.hh"

HPlusPlusEfficiency::HPlusPlusEfficiency(const std::shared_ptr<MatchingModule> imatchModule):
  EfficiencyModule(),
  matchModule(imatchModule)
  ,genSimMuons(0)
  ,genSimElectrons(0)
  ,genSimHPlusPlus(0)
{
}

bool HPlusPlusEfficiency::process()
{
  auto genSim = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::leftDoublyHiggs());
  auto leftHiggs = genSim.getSpecial("LeftHiggs").getParticles();
  
  
  //makes a list of the final daughters of each higgs
  for(const auto &particle : leftHiggs)
  {
    if(particle==particle->finalDaughter())
    {
      particle.push_back(doublyHiggs);
    }
  }
  //groups higgsis by mother and makes a vector of their lepton children
  for(auto it = doublyHiggs.begin(); it != doublyHiggs.end(); it++)
  {
    auto &particle= *it;
    doublyHiggs.erase(it);

    for(auto jt = doublyHiggs.begin(); jt != doublyHiggs.end(); jt++)
    {
      auto &sister= *jt;
      doublyHiggs.erase(jt);
      if(particle->uniqueMother()==sister->uniqueMother())
      {
        decays.push_back(sister->getDaughters().push_back(particle->getDaughters()));
      }
    }
  }

  

//   std::vector<Particle> recoElectronslist, recoMuonsList, negList, posList;

//   for(const auto &particle : particles)
//   {
//     if(particle.isNotNull())
//     {
//       auto type = particle.getType();
//       auto charge = particle.getCharge();

//       if(type == ParticleType::electron())
//       {
//         recoElectronsList.add(particle);
//       }
//       else if(type == ParticleType::muon())
//       {
//         recoMuonsList.add(particle);
//       }

//       if(charge == -1)
//       {
//         negList.add(particle)
//       }
//       else if(charge == 1)
//       {
//         posList.add(particle)
//       }


//     }
//   }

//   return true;
// }

// void HPlusPlusEfficiency::finalize()
// {
//   std::string muonOutputS;tring = std::to_string(recoMuons/(double)genSimMuons);
//   writeText(muonOutputString, "Muon Efficiency");

//   std::string electronOutputString = std::to_string(recoElectrons/(double)genSimElectrons);
//   writeText(electronOutputString, "Electron Efficiency");
}
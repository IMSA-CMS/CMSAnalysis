#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include <iostream>
#include <fstream>
#include <map>

#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

HPlusPlusEfficiency::HPlusPlusEfficiency():
  EfficiencyModule()
{}

//getInput() = make_shared<EventModule>()->getEventInputModule()
//make_shared<EventModule>()->getEventInputModule() returns an InputModule

void HPlusPlusEfficiency::doCounters()
{
  // for (int i = 0; i < 21; i++)
  // {
  //   incrementCounter(genSimDecays.at(i), 0);
  //   for (int j = 0; j < 19; j++)
  //   {
  //     incrementCounter(genSimDecays.at(i) + "->" + recoDecays.at(j), 0);
  //   }
  // }

  HPlusPlusDecayFilter genSimEvent(EventInput::RecoLevel::GenSim);
  std::string genSimDecay = genSimEvent.getState(getInput());

  HPlusPlusDecayFilter recoEvent(EventInput::RecoLevel::Reco);
  std::string recoDecay = recoEvent.getState(getInput());

  std::string decay = genSimDecay + "->" + recoDecay;

  incrementCounter(genSimDecay, 1);
  incrementCounter(decay, 1);
  incrementCounter("r" + recoDecay, 1);

  incrementCounter("events", 1);

  

  // auto genSim = getInput()->getParticles(InputModule::RecoLevel::GenSim);
  // auto reco = getInput()->getParticles(InputModule::RecoLevel::Reco);

  // std::vector<GenSimParticle> pair;

  // for(auto &particle1 : genSim)
  // {
  //   auto particle = GenSimParticle(particle1);
  //   if(particle.getType() == ParticleType::electron() || particle.getType() == ParticleType::muon())
  //   {
  //     if(pair.size() == 0)
  //     {
  //       if(particle.isFinalDaughter() && particle.mother().getType() == ParticleType::photon())
  //       {
  //         pair.push_back(particle);
  //       }
  //     }
  //     else if(pair.size() == 1)
  //     {
  //       if(particle.isFinalDaughter() && particle.mother().getType() == ParticleType::photon() && pair.at(0).mother() == particle.mother())
  //       {
  //         pair.push_back(particle);
  //       }
  //     }
  //   }
  //   if(pair.size() == 2)
  //   {
  //     break;
  //   }
  // }

  // ParticleType type = pair.at(0).getType();

  // std::vector<Particle> rpair;

  // double charge = 0;

  // for(auto &particle : reco)
  // {
  //   if(pair.size() == 0)
  //   {
  //     if(particle.getType() == type)
  //     {
  //       charge = particle.getCharge();
  //       rpair.push_back(particle);
  //     }
  //   }
  //   else if(pair.size() == 1)
  //   {
  //     if(particle.getType() == type && particle.getCharge() == -charge)
  //     {
  //       rpair.push_back(particle);
  //     }
  //   }
  //   if(rpair.size() == 2)
  //   {
  //     break;
  //   }
  // }

  // if(rpair.size() == 0)
  // {
  //   for(int i = 0; i < 2; i++)
  //   {
  //     std::cout << "Eta: " << pair.at(i).getEta() << std::endl;
  //     std::cout << "pT: " << pair.at(i).getPt() << std::endl;
  //   }
  // }
  // else if(rpair.size() == 1)
  // {
  //   for(int i = 0; i < 2; i++)
  //   {
  //     if(pair.at(i).getCharge() == -rpair.at(0).getCharge())
  //     {
  //       std::cout << "Eta: " << pair.at(i).getEta() << std::endl;
  //       std::cout << "pT: " << pair.at(i).getPt() << std::endl;
  //     }
  //   }
  // }

  // std::cout << "-----" << std::endl;
}

void HPlusPlusEfficiency::finalize()
{
  EfficiencyModule::finalize();

  std::string filestr = "\n";

  // for (int i = 0; i < 21; i++)
  // {
  //   for (int j = 0; j < 20; j++)
  //   {
  //     try
  //     {
  //       std::string decay = genSimDecays.at(i) + "->" + recoDecays.at(j);
  //       filestr += std::to_string(getCounter(decay)/(double)getCounter(genSimDecays.at(i)));
  //     }
  //     catch (std::out_of_range&)
  //     {
  //       filestr += "0";
  //     }
  //     if (j != 19)
  //     {
  //       filestr += ",";
  //     }
  //   }
  //   filestr += "\n";
  // }

  // filestr += "\n\n";

  // for (int i = 0; i < 19; i++)
  // {
  //   for (int j = 0; j < 21; j++)
  //   {
  //     try
  //     {
  //       std::string decay = genSimDecays.at(j) + "->" + recoDecays.at(i);
  //       filestr += std::to_string(getCounter(decay)/(double)getCounter(recoDecays.at(i)));
  //     }
  //     catch (std::out_of_range&)
  //     {
  //       filestr += "0";
  //     }
  //     if (j != 20)
  //     {
  //       filestr += ",";
  //     }
  //   }
  //   filestr += "\n";
  // }

  // filestr += "\n";
  
  for (int i = 0; i < 20; i++)
  {
    try
    {
      filestr += std::to_string(getCounter("r" + recoDecays.at(i))/(double)getCounter("events"));
    }
    catch (std::out_of_range&)
    {
      filestr += "0";
    }
    if (i != 19)
    {
      filestr += ",";
    }
  }

  filestr += "\n\n";

  // std::ofstream file("data.txt");
  // file << filestr;
  // file.close();

  std::cout << filestr << std::endl;
}
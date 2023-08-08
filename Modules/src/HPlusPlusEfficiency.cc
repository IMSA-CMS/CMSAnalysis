#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include <iostream>

#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"

HPlusPlusEfficiency::HPlusPlusEfficiency():
  EfficiencyModule()
{}
bool HPlusPlusEfficiency::process()
{
  std::string cstr[] = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu"};

  auto genSim = getInput()->getParticles(EventInput::RecoLevel::GenSim);
  auto reco = getInput()->getLeptons(EventInput::RecoLevel::Reco);

  // Reco stuff
  HPlusPlusDecayFilter recoEvent(EventInput::RecoLevel::Reco);
  std::string recoDecay = recoEvent.getState(genSim, reco);

  for (int i = 0; i < 12; i++)
  {
    if (i < 6)
    {
      incrementCounter("g" + cstr[i], 0);
      for (int j = 6; j < 12; j++)
      {
        incrementCounter(cstr[j] + "<-" + cstr[i], 0);
      }
    }
    incrementCounter("r" + cstr[i], 0);
  }

  // incrementCounter("gnone", 0);
  // incrementCounter("rnone", 0);
  // incrementCounter("overflow", 0);
  // incrementCounter("underflow", 0);
  // incrementCounter("none", 0);

  if(recoDecay.compare("none") == 0)
  {
  //   if(recoDecay.size() < 4)
  //   {
  //     incrementCounter("underflow", 1);
  //   }
  //   else if(recoDecay.size() > 5)
  //   {
  //     incrementCounter("overflow", 1);
  //   }
  //   else
  //   {
  //     incrementCounter("none", 1);
  //   }
  }
  else
  {
    incrementCounter(recoDecay, 1);
  }

  // GenSim stuff
  HPlusPlusDecayFilter genSimEvent(EventInput::RecoLevel::GenSim);
  std::string genSimDecay = genSimEvent.getState(genSim, reco);

  if  (genSimDecay.compare("none") != 0)
  {
    incrementCounter("g" + genSimDecay, 1);
  }

  if (recoDecay.compare("none") != 0 && genSimDecay.compare("none") != 0)
  {
    incrementCounter("r" + recoDecay, 1);
  }
  if(recoDecay.compare(genSimDecay) != 0 && recoDecay.compare("none") != 0 && genSimDecay.compare("none") != 0)
  {
    incrementCounter(recoDecay + "<-" + genSimDecay, 1);
  }
  // if(recoDecay.compare("none") != 0 && genSimDecay.compare("none") == 0)
  // {
  //   incrementCounter("f" + recoDecay, 1);
  // }
  
  // else if (genSimDecay.compare("none") == 0 && recoDecay.compare("none") != 0)
  // {
  //   incrementCounter("f" + recoDecay, 1);
  // }
  return true;
}

void HPlusPlusEfficiency::finalize()
{
  std::string cstr[] = {"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu"};

  std::string d00 = std::to_string(getCounter("reeee")/(double)getCounter("geeee"));
  writeText(d00, "eeee -> eeee");
  std::string d01 = std::to_string(getCounter("eee<-eeee")/(double)getCounter("geeee"));
  writeText(d01, "eeee -> eee");
  std::string d02 = std::to_string(1 - (getCounter("reeee") + getCounter("eee<-eeee"))/(double)getCounter("geeee"));
  writeText(d02, "eeee lost");

  std::string d10 = std::to_string(getCounter("reeeu")/(double)getCounter("geeeu"));
  writeText(d10, "eeeu -> eeeu");
  std::string d11 = std::to_string(getCounter("eee<-eeeu")/(double)getCounter("geeeu"));
  writeText(d11, "eeeu -> eee");
  std::string d12 = std::to_string(getCounter("eeu<-eeeu")/(double)getCounter("geeeu"));
  writeText(d12, "eeeu -> eeu");
  std::string d13 = std::to_string(getCounter("eue<-eeeu")/(double)getCounter("geeeu"));
  writeText(d13, "eeeu -> eue");
  std::string d14 = std::to_string(1 - (getCounter("reeeu") + getCounter("eee<-eeeu") + getCounter("eeu<-eeeu") + getCounter("eue<-eeeu"))/(double)getCounter("geeeu"));
  writeText(d14, "eeeu lost");

  std::string d20 = std::to_string(getCounter("reeuu")/(double)getCounter("geeuu"));
  writeText(d20, "eeuu -> eeuu");
  std::string d21 = std::to_string(getCounter("eeu<-eeuu")/(double)getCounter("geeuu"));
  writeText(d21, "eeuu -> eeu");
  std::string d22 = std::to_string(getCounter("uue<-eeuu")/(double)getCounter("geeuu"));
  writeText(d22, "eeuu -> uue");
  std::string d23 = std::to_string(1 - (getCounter("reeuu") + getCounter("eeu<-eeuu") + getCounter("uue<-eeuu"))/(double)getCounter("geeuu"));
  writeText(d23, "eeuu lost");

  std::string d30 = std::to_string(getCounter("reueu")/(double)getCounter("geueu"));
  writeText(d30, "eueu -> eueu");
  std::string d31 = std::to_string(getCounter("eue<-eueu")/(double)getCounter("geueu"));
  writeText(d31, "eueu -> eue");
  std::string d32 = std::to_string(getCounter("euu<-eueu")/(double)getCounter("geueu"));
  writeText(d32, "eueu -> euu");
  std::string d33 = std::to_string(1 - (getCounter("reueu") + getCounter("eue<-eueu") + getCounter("euu<-eueu"))/(double)getCounter("geueu"));
  writeText(d33, "eueu lost");

  std::string d40 = std::to_string(getCounter("reuuu")/(double)getCounter("geuuu"));
  writeText(d40, "euuu -> euuu");
  std::string d41 = std::to_string(getCounter("euu<-euuu")/(double)getCounter("geuuu"));
  writeText(d41, "euuu -> euu");
  std::string d42 = std::to_string(getCounter("uue<-euuu")/(double)getCounter("geuuu"));
  writeText(d42, "euuu -> uue");
  std::string d43 = std::to_string(getCounter("uuu<-euuu")/(double)getCounter("geuuu"));
  writeText(d43, "euuu -> uuu");
  std::string d44 = std::to_string(1 - (getCounter("reuuu") + getCounter("euu<-euuu") + getCounter("uue<-euuu") + getCounter("uuu<-euuu"))/(double)getCounter("geuuu"));
  writeText(d44, "euuu lost");

  std::string d50 = std::to_string(getCounter("ruuuu")/(double)getCounter("guuuu"));
  writeText(d50, "uuuu -> uuuu");
  std::string d51 = std::to_string(getCounter("uuu<-uuuu")/(double)getCounter("guuuu"));
  writeText(d51, "uuuu -> uuu");
  std::string d52 = std::to_string(1 - (getCounter("ruuuu") + getCounter("uuu<-uuuu"))/(double)getCounter("guuuu"));
  writeText(d52, "uuuu lost");

  for (int i = 0; i < 6; i++)
  {
    std::string f0 = std::to_string(getCounter("r" + cstr[i])/(double)getCounter(cstr[i]));
    writeText(f0, cstr[i] + " <- " + cstr[i]);

    std::string f = std::to_string(1 - (getCounter("r" + cstr[i])/(double)getCounter(cstr[i])));
    writeText(f, cstr[i] + " fake");
  }

  std::string f00 = std::to_string(getCounter("eee<-eeee")/(double)getCounter("eee"));
  writeText(f00, "eee <- eeee");
  std::string f01 = std::to_string(getCounter("eee<-eeeu")/(double)getCounter("eee"));
  writeText(f01, "eee <- eeeu");
  std::string f02 = std::to_string(1 - (getCounter("eee<-eeee") + getCounter("eee<-eeeu"))/(double)getCounter("eee"));
  writeText(f02, "eee fake");

  std::string f10 = std::to_string(getCounter("eeu<-eeeu")/(double)getCounter("eeu"));
  writeText(f10, "eeu <- eeeu");
  std::string f11 = std::to_string(getCounter("eeu<-eeuu")/(double)getCounter("eeu"));
  writeText(f11, "eeu <- eeuu");
  std::string f12 = std::to_string(1 - (getCounter("eeu<-eeeu") + getCounter("eeu<-eeuu"))/(double)getCounter("eeu"));
  writeText(f12, "eeu fake");

  std::string f20 = std::to_string(getCounter("eue<-eeeu")/(double)getCounter("eue"));
  writeText(f20, "eue <- eeeu");
  std::string f21 = std::to_string(getCounter("eue<-eueu")/(double)getCounter("eue"));
  writeText(f21, "eue <- eueu");
  std::string f22 = std::to_string(1 - (getCounter("eue<-eeeu") + getCounter("eue<-eueu"))/(double)getCounter("eue"));
  writeText(f22, "eue fake");

  std::string f30 = std::to_string(getCounter("euu<-eueu")/(double)getCounter("euu"));
  writeText(f30, "euu <- eueu");
  std::string f31 = std::to_string(getCounter("euu<-euuu")/(double)getCounter("euu"));
  writeText(f31, "euu <- euuu");
  std::string f32 = std::to_string(1 - (getCounter("euu<-eueu") + getCounter("euu<-euuu"))/(double)getCounter("euu"));
  writeText(f32, "euu fake");

  std::string f40 = std::to_string(getCounter("uue<-eeuu")/(double)getCounter("uue"));
  writeText(f40, "uue <- eeuu");
  std::string f41 = std::to_string(getCounter("uue<-euuu")/(double)getCounter("uue"));
  writeText(f41, "uue <- euuu");
  std::string f42 = std::to_string(1 - (getCounter("uue<-eeuu") + getCounter("uue<-euuu"))/(double)getCounter("uue"));
  writeText(f42, "uue fake");

  std::string f50 = std::to_string(getCounter("uuu<-euuu")/(double)getCounter("uuu"));
  writeText(f50, "uuu <- euuu");
  std::string f51 = std::to_string(getCounter("uuu<-uuuu")/(double)getCounter("uuu"));
  writeText(f51, "uuu <- uuuu");
  std::string f52 = std::to_string(1 - (getCounter("uuu<-euuu") + getCounter("uuu<-uuuu"))/(double)getCounter("uuu"));
  writeText(f52, "uuu fake");
}
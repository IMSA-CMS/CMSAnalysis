#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include <iostream>
#include <fstream>
#include <map>

#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/TableWriter.hh"
#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/LatexTable.hh"
#include "CMSAnalysis/Utility/interface/PowerpointTable.hh"

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

  incrementCounter("eeeeTotal", 0);
  incrementCounter("eeeeCut", 0);

  if (recoDecay == "eeee")
  {
    incrementCounter("eeeeTotal", 1);

    for (auto p : getInput()->getParticles(EventInput::RecoLevel::Reco))
    {
      //std::cout << "PT: " << std::to_string(p.getPt()) << std::endl;

      if (p.getPt() < 10)
      {
        //std::cout << "CUT!" << std::endl;
        incrementCounter("eeeeCut", 1);
        break;
      }
    }
  }

  std::string decay = genSimDecay + "->" + recoDecay;

  incrementCounter(genSimDecay, 1);
  incrementCounter(decay, 1);
  incrementCounter("r" + recoDecay, 1);

  incrementCounter("events", 1);
}

void HPlusPlusEfficiency::finalize()
{
  EfficiencyModule::finalize();

  bool hardCoded = true;

  std::vector<std::vector<std::string>> separatedValues;
  std::vector<std::vector<std::string>> wholeValues;

  std::vector<double> genDecayCounts;
  double eventNum;

  if (hardCoded)
  {
    genDecayCounts = genSimDecayCounts;
    
    eventNum = numEvents;
  }
  else
  {
    for (auto decay : genSimDecays)
    {
      try
      {
        genDecayCounts.push_back(getCounter(decay));
      }
      catch(std::out_of_range&)
      {
        genDecayCounts.push_back(0);
      }
    }

    eventNum = getCounter("events");
  }

  for (int i = 0; i < 21; i++)
  {
    for (int j = 0; j < 19; j++)
    {
      std::string value;

      try
      {
        std::string decay = genSimDecays.at(i) + "->" + recoDecays.at(j);
        if (genDecayCounts.at(i) == 0)
        {
          value = "0";
        }
        else
        {
          value = std::to_string(getCounter(decay)/genDecayCounts.at(i));
        }
      }
      catch (std::out_of_range&)
      {
        value = "0";
      }
      
      std::vector<std::string> input = {recoDecays.at(j), genSimDecays.at(i), value};
      separatedValues.push_back(input);
    }
  }
  
  for (int i = 0; i < 19; i++)
  {
    std::string value;

    try
    {
      value = std::to_string(getCounter("r" + recoDecays.at(i))/eventNum);
    }
    catch (std::out_of_range&)
    {
      value = "0";
    }

    std::vector<std::string> input = {recoDecays.at(i), "all", value};
    wholeValues.push_back(input);
  }

  std::cout << "eeee cut / eeee total : " << std::to_string(getCounter("eeeeCut") / getCounter("eeeeTotal")) << std::endl;

  // This is to find hard-coded denominators (use with no cuts applied)
  // std::string str;
  // for (int i = 0; i < 21; i++)
  // {
  //   try
  //   {
  //     str += std::to_string(getCounter(genSimDecays.at(i)));
  //   }
  //   catch (std::out_of_range&)
  //   {
  //     str += "0";
  //   }
  //   if (i != 20)
  //   {
  //     str += ", ";
  //   }
  // }
  // std::cout << str << std::endl;

  // PowerpointTable table1;
  // PowerpointTable table2;

  // TableData separatedData(separatedValues);
  // TableData wholeData(wholeValues);

  // auto p1 = std::make_shared<TableData>(separatedData);
  // auto p2 = std::make_shared<TableData>(wholeData);

  // std::string filename1 = "CMSAnalysis/Modules/separatedTable.txt";
  // std::string filename2 = "CMSAnalysis/Modules/wholeTable.txt";

  // std::ofstream file1;
  // std::ofstream file2;

  // file1.open(filename1);
  // file2.open(filename2);

  // table1.makeTable(p1, std::cout);
  // table2.makeTable(p2, std::cout);

  // file1.close();
  // file2.close();
}
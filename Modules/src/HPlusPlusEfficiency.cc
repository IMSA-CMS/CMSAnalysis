#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <iomanip>
#include <sstream>

#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/TableWriter.hh"
#include "CMSAnalysis/Utility/interface/TableData.hh"
#include "CMSAnalysis/Utility/interface/LatexTable.hh"
#include "CMSAnalysis/Utility/interface/PowerpointTable.hh"
#include "CMSAnalysis/Utility/interface/TextTable.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"


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

//   for (int i = 0; i < 21; i++) // genSim decays
//   {
//     for (int j = 0; j < 19; j++) // reco decays
//     {
//       std::string value;
//
//       try
//       {
//         std::string decay = genSimDecays.at(i) + "->" + recoDecays.at(j);
//         if (genDecayCounts.at(i) == 0)
//         {
//           value = "0";
//         }
//         else
//         {
//           value = std::to_string(getCounter(decay)/genDecayCounts.at(i));
//         }
//       }
//       catch (std::out_of_range&)
//       {
//         value = "0";
//       }
//
//       std::vector<std::string> input = {recoDecays.at(j), genSimDecays.at(i), value};
//       separatedValues.push_back(input);
//     }
//   }
//
  // Get list of all the reco channels and add partial channels to it
  std::vector<std::string> recoChannels = genSimDecays;
  const std::vector<std::string> partialChannels{
    "eee_", "eeu_", "eet_", "eue_", "euu_", "eut_",
    "ete_", "etu_", "ett_", "uue_", "uuu_", "uut_",
    "ute_", "utu_", "utt_", "tte_", "ttu_", "ttt_",
    "ee__", "eu__", "et__", "uu__", "ut__", "tt__",
    "e_e_", "e_u_", "e_t_", "u_u_", "u_t_", "t_t_",
    "e___", "u___", "t___", "____", "none"
  };
  recoChannels.insert(recoChannels.end(),
                      partialChannels.begin(), partialChannels.end());

  for (const auto& genChannel : genSimDecays)
  {
    std::vector<std::string> row;
    double rowTotal = 0;
    for (const auto& recoChannel : recoChannels)
    {
      // Construct the counter decay string for the current gen and reco channels
      const std::string decay =
          "GenSim " + genChannel + "->Reco " + recoChannel;
      rowTotal += getCounter(decay);
      // get raw event count, cast to int, convert to string for the table
      row.push_back(std::to_string(
          static_cast<unsigned long long>(getCounter(decay)))); 
    }
    // Normalize reconstructed counts after the complete row total is known.
    for (auto& cell : row)
    {
      const double fraction = rowTotal > 0 ? std::stod(cell) / rowTotal : 0.0;
      std::ostringstream formatted;
      formatted << std::fixed << std::setprecision(4) << fraction;
      cell = formatted.str();
    }
    row.push_back(std::to_string(
        static_cast<unsigned long long>(rowTotal)));
    separatedValues.push_back(row);
  }
  recoChannels.push_back("Total");

  for (int i = 0; i < 19; i++) // reco decays again 
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
  TextTable table2;

  // TableData separatedData(separatedValues);
  // TableData wholeData(wholeValues);
  TableData wholeData(separatedValues, recoChannels, genSimDecays);

  // auto p1 = std::make_shared<TableData>(separatedData);
  auto p2 = std::make_shared<TableData>(wholeData);

  // std::string filename1 = "CMSAnalysis/Modules/separatedTable.txt";
  std::string filename2 = Utility::getBasePath() + "Output/wholeTable.txt";

  // std::ofstream file1;
  std::ofstream file2;

  // file1.open(filename1);
  file2.open(filename2);
  if (!file2)
  {
    throw std::runtime_error("Cannot open table output: " + filename2);
  }

  // table1.makeTable(p1, std::cout);
  table2.makeTable(p2, std::cout);
  table2.makeTable(p2, file2);

  // file1.close();
  file2.close();
}
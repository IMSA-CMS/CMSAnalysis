#include "CMSAnalysis/DataCollection/interface/AnalyzerOptions.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexPlan.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusMassPlan.hh"
#include "CMSAnalysis/DataCollection/interface/InvariantMassPlan.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetBackgroundPlan.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/MassAcceptancePlan.hh"
#include "CMSAnalysis/DataCollection/interface/MassResolutionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/Trigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerPlan.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimPlan.hh"
#include "CMSAnalysis/DataCollection/interface/FileStripPlan.hh"
#include "CMSAnalysis/DataCollection/interface/MLVariablesPlan.hh"

AnalyzerOptions::AnalyzerOptions()
{
  analysisPlans["DisplacedVertex"] = new DisplacedVertexPlan();
  analysisPlans["HiggsBackground"] = new HiggsBackgroundPlan();
  analysisPlans["InvariantMass"] = new InvariantMassPlan();
  analysisPlans["LeptonJetBackground"] = new LeptonJetBackgroundPlan();
  analysisPlans["MassResolution"] = new MassResolutionPlan();
  analysisPlans["LeptonJetReconstruction"] = new LeptonJetReconstructionPlan();
  analysisPlans["MassAcceptance"] = new MassAcceptancePlan();
  analysisPlans["Trigger"] = new TriggerPlan();
  analysisPlans["GenSim"] = new GenSimPlan();
  analysisPlans["FileStrip"] = new FileStripPlan();
  analysisPlans["DataStrip"] = new MLVariablesPlan();
}

std::string AnalyzerOptions::pickfileInterface()
{
  std::cout << "\n                                        Pickfile Generator                                         \n"
            << "===================================================================================================\n";
  // Ask for process name and idtypes
  std::vector<std::string> namesOfProcesses = processNames();

  std::vector<std::pair<std::string, std::string>> pickfileInfo = getProcessAndIDTypes(namesOfProcesses);

  // Ask for the filename
  std::string filename = takeFilename();
  std::string filenameString = "Filename";
  pickfileInfo.push_back(std::pair<std::string, std::string>(filenameString, filename));

  // Print current status, ask for any changes
  currentStatus(&(pickfileInfo), namesOfProcesses);
  std::cout << "Generating pickfile " << filename << "... \n";
  makePickfile(pickfileInfo);

  // Finally, return the newly created input file name
  return pickfileInfo.back().second;
}

std::vector<std::string> AnalyzerOptions::processNames()
{
  std::vector<std::string> processes;
  std::ifstream textfile("textfiles/processes.txt");
  if (!(textfile))
  {
    throw std::runtime_error("Make sure there is a file called processes.txt in the folder textfiles. Also, make sure you're running from the bin folder");
  }
  std::string line;

  // Looks line by line through the file to get the names of the processes
  while (getline(textfile, line))
  {
    if (line.empty())
    {
      // Go to the next line if the current line is empty
      continue;
    }
    else if ((line.find(':') == std::string::npos))
    {
      // If it doesn't have a colon, it must be the name of a process. Put it into the process names vector.
      processes.push_back(line);
      continue;
    }
  }
  textfile.close();
  return processes;
}

std::vector<std::pair<std::string, std::string>> AnalyzerOptions::getProcessAndIDTypes(std::vector<std::string> processes)
{
  // Asks for the user to select a process, and then stores this information in a vector of pairs
  std::pair<std::string, std::string> processName = promptInput("Process", processes);
  std::vector<std::pair<std::string, std::string>> pickfileInfo;
  pickfileInfo.push_back(processName);

  // Ask for each IDType separately
  std::vector<IDType> idtypes = findProcessIDTypes(processName.second);
  std::cout << "Process " << processName.second << " has " << idtypes.size() << " IDTypes.\n\n";

  for (auto &idtype : idtypes)
  {
    std::pair<std::string, std::string> newCategory = promptInput(idtype.getName(), idtype.getCategories());
    pickfileInfo.push_back(newCategory);
  }
  return pickfileInfo;
}

std::string AnalyzerOptions::takeFilename()
{
  bool validFilename = false;
  std::string filename;
  while (!(validFilename))
  {
    // Ask for a filename
    std::cout << "Enter a file name (with the .txt) and its path, as started below. \nfilename: bin/";
    filename = takeInput();
    validFilename = checkFilename(filename);
  }
  return filename;
}

bool AnalyzerOptions::checkFilename(std::string name)
{
  if (name.substr(name.length() - 4, 4) != ".txt")
  {
    std::cout << "Please enter a file name with .txt\n\n";
    return false;
  }
  return true;
}

void AnalyzerOptions::currentStatus(std::vector<std::pair<std::string, std::string>> *pickfileInfo, std::vector<std::string> processNames)
{
  // Note: passing in a pointer to the pickfileinfo because it would be easier to change it directly in this function

  std::string change;
  // This loop keeps running until change == "N". But, it is guaranteed to run once.
  do
  {
    // Output the information, and create a vector of names in case the user wants to change something
    std::cout << "Here is the current information entered: \n";
    std::vector<std::string> namesVector;
    for (const auto &item : *(pickfileInfo))
    {
      std::cout << '\t' << item.first << ": " << item.second << "\n";
      namesVector.push_back(item.first);
    }

    // Ask for changes
    std::cout << "\nWould you like to change anything? \n";
    change = takeSingleInput({"Y", "N"}, "(Y or N): ");

    if (change == "Y")
    {
      // If the user wants to change something, ask what they want to change
      std::cout << "Which of the following would you like to change? Type \"Back\" to go back.\n"
                << separateVector(namesVector, '\t') << std::endl;
      namesVector.push_back("Back");
      std::string categoryToChange = takeSingleInput(namesVector, "Pick one or type \"Back\": ");
      if (categoryToChange == "Back")
      {
        continue;
      }
      if (categoryToChange == "Filename")
      {
        // Change the file name (which will always be at the end of the vector)
        pickfileInfo->back().second = takeFilename();
      }
      else if (categoryToChange == "Process")
      {
        // If the process changes, then the idtypes must change, too. Both must be deleted.
        // Save the filename before clearing the whole vector
        std::pair<std::string, std::string> filename = pickfileInfo->back();
        pickfileInfo->clear();

        // Ask for new process and idtypes
        *pickfileInfo = getProcessAndIDTypes(processNames);
        // finish the vector by adding in the filename to the end.
        pickfileInfo->push_back(filename);
      }
      else
      {
        // anything else must be an idtype.
        std::vector<IDType> idtypes = findProcessIDTypes((*pickfileInfo)[0].second);
        // Finds out what index of pickfileInfo the selected IDType is at by looping through the vector
        int i = 0;
        for (auto &item : (*pickfileInfo))
        {
          if (item.first == categoryToChange)
          {
            break;
          }
          i++;
        }

        // Prompts the user's input for what the idtype should be changed to, then changes the correct entry
        // in pickfileinfo.
        for (auto &idtype : idtypes)
        {
          if (idtype.getName() == categoryToChange)
          {
            std::pair<std::string, std::string> newIdtype = promptInput(idtype.getName(), idtype.getCategories());
            (*pickfileInfo)[i].second = newIdtype.second;
          }
        }
      }
    }
    if (change == "N")
    {
      std::cout << "Would you like to generate the pickfile?\n";
      std::string generate = takeSingleInput({"Y", "N"}, "(Y or N): ");
      if (generate == "N")
      {
        change = "Y"; // This makes it go through the do-while again
      }
    }
  } while (change == "Y");
}

std::string AnalyzerOptions::separateVector(std::vector<std::string> list, char separator)
{
  std::ostringstream namesToOutput;
  for (auto &name : list)
  {
    namesToOutput << name << separator;
  }

  std::string outputString = namesToOutput.str();
  outputString.pop_back();
  return outputString;
}

bool AnalyzerOptions::checkInput(std::string input, std::vector<std::string> list)
{
  for (auto &item : list)
  {
    if (item == input)
    {
      return true;
    }
  }
  return false;
}

std::vector<IDType> AnalyzerOptions::findProcessIDTypes(std::string process)
{
  std::vector<IDType> idtypes;
  std::ifstream processes("textfiles/processes.txt");
  std::string line;
  bool foundProcess = false;
  std::vector<std::string> IDCategories;

  // Reading processes.txt line by line
  while (getline(processes, line))
  {
    if (line.empty())
    {
      // Go to the next line if this one is empty
      continue;
    }
    else if (foundProcess == true)
    {
      if (line.find(':') == std::string::npos)
      {
        // If the process name has already been passed, but there is no colon in this line, then it
        // must be the name of another process -- there are no more idtypes remaining in the process
        // we're looking at. We can return the vector of idtypes.
        processes.close();
        break;
        // return idtypes;
      }
      // If the process name has been found and this one has a colon, then it has information about
      // the process's idtypes.

      int nameLength = line.find(':');
      std::string name = line.substr(0, nameLength);
      std::string types = line.substr(nameLength + 1, line.length() - 1);
      std::istringstream typeStream(types);
      std::string category;

      // Separate the line by tabs and add each chunk to the categories vector
      while (getline(typeStream, category, '\t'))
      {
        if (category.empty())
        {
          continue;
        }
        IDCategories.push_back(category);
      }

      // Add the new idtype to the vector of idtypes.
      IDType newType(name, IDCategories);
      idtypes.push_back(newType);

      IDCategories.clear();
    }
    else if (line == process)
    {
      // If the line is the same as the process name, then the next few lines should be its idtypes.
      foundProcess = true;
      continue;
    }
  }
  return idtypes;
}

std::string AnalyzerOptions::takeInput()
{
  std::string input;
  std::cin >> input;
  // Outputs a newline character so the terminal is easier to read
  std::cout << '\n';
  return input;
}

std::pair<std::string, std::string> AnalyzerOptions::promptInput(std::string name, std::vector<std::string> categories)
{
  bool processInput = false;
  std::string selected;
  // asks the until the user has given a answer found in processes.txt
  while (processInput == false)
  {
    std::cout << "Please choose a " << name << " from the following \n";
    std::cout << separateVector(categories, '\t') << "\n";
    std::cout << "Choose one: ";
    selected = takeInput();
    processInput = checkInput(selected, categories);
    if (processInput == false)
    {
      std::cout << '\'' << selected << "\' is not found in an entry from the list. Please try again. \n\n";
    }
  }
  std::pair<std::string, std::string> inputToReturn(name, selected);
  return inputToReturn;
}

std::string AnalyzerOptions::takeSingleInput(std::vector<std::string> categories, std::string promptString)
{
  bool processInput = false;
  std::string input;
  while (processInput == false)
  {
    // Prompt user input
    std::cout << promptString;
    input = takeInput();
    processInput = checkInput(input, categories);
    if (processInput == false)
    {
      std::cout << "Please check your input and try again. \n";
    }
  }
  return input;
}

void AnalyzerOptions::makePickfile(std::vector<std::pair<std::string, std::string>> info)
{
  // Filename is the last entry in the vector
  std::string name = info.back().second;

  // create the textfile
  std::ofstream pickfile(name);
  // find the process name and write it into the file
  name = info[0].second;
  pickfile << name << '\n';

  // write each idtype into the file
  std::ostringstream types;
  for (auto &item : info)
  {
    if (!((item.first == "Process") || (item.first == "Filename")))
    {
      types << item.first << ":\t" << item.second << "\n";
    }
  }
  std::string typeString = types.str();
  // taking out the final newline character
  typeString.pop_back();
  pickfile << typeString;
  pickfile.close();
  std::cout << "Generated Pickfile! \n";
  std::cout << "===================================================================================================\n\n";
}

std::string AnalyzerOptions::checkSelectedAnalysis(std::string analysis)
{
  std::string plan;
  if (analysisPlans.find(analysis) == analysisPlans.end())
  {
    if (analysis.empty())
    {
      std::cout << "Please choose an analysis from the following: \n\n";
    }
    else
    {
      std::cout << "The selected analysis \"" << analysis << "\" does not exist. \nPlease choose from the following: \n";
    }
    // A vector that contains the keys in the plan vector
    std::vector<std::string> planNames;
    for (auto &entry : analysisPlans)
    {
      planNames.push_back(entry.first);
    }
    std::cout << separateVector(planNames, '\n') << '\n';
    std::string newAnalysis = takeSingleInput(planNames, "Choose one: ");
    plan = newAnalysis;
  }
  else
  {
    plan = analysis;
  }
  return plan;
}
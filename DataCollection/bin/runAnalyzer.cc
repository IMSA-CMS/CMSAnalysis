#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "CMSAnalysis/DataCollection/bin/massResolutionAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/HPlusPlusMassAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/leptonJetReconstructionAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/displacedVertexAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/massAcceptanceAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/triggerAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/invariantMassAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/HiggsBackgroundAnalysis.cc"
#include "CMSAnalysis/DataCollection/bin/LeptonJetBackgroundAnalysis.cc"

namespace fs = boost::filesystem;

// pickfile creating interface
std::string pickfileInterface(); 

// Reads processes.txt and returns a string with the names of the processes in the file
std::vector<std::string> processNames();

// gets user input for and checks the process and idtypes of the pickfile
std::vector<std::pair<std::string, std::string>> getProcessAndIDTypes(std::vector<std::string> processes);

// takes the file name and calls the check function, returns the file name in a string
std::string takeFilename();

// checks if the file name is valid
bool checkFilename(std::string name);

// Print current status and ask for any changes
void currentStatus(std::vector<std::pair<std::string, std::string>>* pickfileInfo, std::vector<std::string> processNames);

// Converts the map to a pickfile
void makePickfile(std::vector<std::pair<std::string, std::string>> info);

// Fills the idtypes of the selected process
std::vector<IDType> findProcessIDTypes(std::string process);

// Takes a vector of strings and outputs a string with the contents of the vector tab separated
std::string separateVector(std::vector<std::string> list, char separator);

// Checks whether the selected item was in the list 
bool checkInput(std::string input, std::vector<std::string> list);

// A function that gets the user's input, adds a newline character and converts the string to lowercase.
std::string takeInput();

// A function that prompts for the user's input, and returns a pair with the user's input and corresponding category
std::pair<std::string, std::string> promptInput(std::string name, std::vector<std::string> categories);

// Takes the user's input and checks it. Only returns the user's input.
std::string takeSingleInput(std::vector<std::string> categories, std::string promptString);


// Pickfile selecting interface
void pickfileSelector();


int main(int argc, char **argv) {
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1);
  parser.parseArguments (argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");

  if (outputFile.empty())
    {
      outputFile = "default.root";
    }

  unsigned outputEvery = parser.integerValue("outputEvery");

  // USING ANALYER FOR H++ THAT WON'T CHANGE: CHANGE BACK
  // TO hPlusPlusMassAnalysis() when ready -- [09/17]
  //Analyzer analyzer = displacedVertexAnalysis();
  //Analyzer analyzer = higgsBackgroundAnalysis();
  //Analyzer analyzer = invariantMassAnalysis();
  //Analyzer analyzer = leptonJetBackgroundAnalysis();
  Analyzer analyzer = leptonJetReconstructionAnalysis();
  //Analyzer analyzer = massAcceptanceAnalysis();
  //Analyzer analyzer = massResolutionAnalysis();
  //Analyzer analyzer = triggerAnalysis();

  if (inputFile.empty())
  {
    // inputFile = "textfiles/pickFiles.txt";
    inputFile = pickfileInterface();
    // pickfileSelector();
  }

  std::cout << "Reading input file " << inputFile << std::endl;
  analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  return 0;
}

std::string pickfileInterface()
{
  std::cout << "===================================================================================================\n"
            << "                                        Pickfile Generator                                         \n"
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

  // Finally, return the newly created input file
  return pickfileInfo.back().second;
}

std::vector<std::string> processNames()
{
  std::vector<std::string> processes;
  std::ifstream textfile("textfiles/processes.txt");
  if (!(textfile))
  {
    throw std::runtime_error("Make sure there is a file called processes.txt in the folder textfiles.");
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

std::vector<std::pair<std::string, std::string>> getProcessAndIDTypes(std::vector<std::string> processes)
{
  // Asks for the user to select a process, and then stores this information in a vector of pairs
  std::pair<std::string, std::string> processName = promptInput("Process", processes);
  std::vector<std::pair<std::string, std::string>> pickfileInfo;
  pickfileInfo.push_back(processName);

  // Ask for each IDType separately
  std::vector<IDType> idtypes = findProcessIDTypes(processName.second);
  std::cout << "Process " << processName.second << " has " << idtypes.size() << " IDTypes.\n\n";

  for (auto& idtype : idtypes)
  {
    std::pair<std::string, std::string> newCategory = promptInput(idtype.getName(), idtype.getCategories());
    pickfileInfo.push_back(newCategory);
  }
  return pickfileInfo;
}

std::string takeFilename()
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

bool checkFilename(std::string name)
{
  // Check if the filename entered has .txt, making it valid
  if (name.substr(name.length()-4, 4) != ".txt")
  {
    std::cout << "Please enter a file name with .txt\n\n";
    return false;
  }
  
  // Separate the string by slashes
  std::istringstream is(name);
  std::string folder;
  std::string filePath;
  while (std::getline(is, folder, '/'))
  {
    if (folder.find(".txt") == std::string::npos)
    {
      // If the string does not have .txt in it -- meaning it is a directory
      filePath = filePath + folder;
      fs::path directory(filePath);
      if (!(fs::exists(directory) && fs::is_directory(directory)))
      {
        std::cout << "Directory " << folder << "/ does not exist. Please try again.\n\n";
        return false;
      }
    }
    else
    // If .txt is found
    {
      fs::path file(name);
      // Check if the file exists
      if (fs::exists(file))
      {
        // If it exists, ask if the user wants to overwrite the file. 
        std::cout << name << " already exists. Continue and overwrite this file?\n";
        std::string proceed = takeSingleInput({"Y", "N"}, "(Y or N): ");
        if (proceed == "N")
        {
          return false;
        }
      }
    }
  }
  return true;
}

void currentStatus(std::vector<std::pair<std::string, std::string>>* pickfileInfo, std::vector<std::string> processNames)
{
  // Note: passing in a pointer to the pickfileinfo because it would be easier to change it directly in this function

  std::string change;
  // This loop keeps running until change == "N". But, it is guaranteed to run once. 
  do
  {
    // Output the information, and create a vector of names in case the user wants to change something
    std::cout << "Here is the current information entered: \n";
    std::vector<std::string> namesVector;
    for (const auto& item : *(pickfileInfo))
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
        for (auto& item : (*pickfileInfo))
        {
          if (item.first == categoryToChange)
          {
            break;
          }
          i++;
        }

        // Prompts the user's input for what the idtype should be changed to, then changes the correct entry
        // in pickfileinfo.
        for (auto& idtype : idtypes)
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

std::string separateVector(std::vector<std::string> list, char separator)
{
  std::ostringstream namesToOutput;
  for (auto& name : list)
  {
    namesToOutput << name << separator;
  }

  std::string outputString = namesToOutput.str();
  outputString.pop_back();
  return outputString;
}

bool checkInput(std::string input, std::vector<std::string> list)
{
  for (auto& item : list)
  {
    if (item == input)
    {
      return true;
    }
  }
  return false;
}

std::vector<IDType> findProcessIDTypes(std::string process)
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
      std::string types = line.substr(nameLength+1, line.length()-1);
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

std::string takeInput()
{
  std::string input;
  std::cin >> input;
  // Outputs a newline character so the terminal is easier to read
  std::cout << '\n';
/*  
  int i = 0;
  // converts the string to lowercase for more user friendliness
  while (i < input.length())
  {
    if (isupper(input[i]))
    {
      input[i] = tolower(input[i]);
    }
  }
*/
  return input;
}

std::pair<std::string, std::string> promptInput(std::string name, std::vector<std::string> categories)
{
  bool processInput = false;
  std::string selected;
  // asks the until the user has given a answer found in processes.txt
  while (processInput == false)
  {
    std::cout << "Please choose a " << name << " from the following \n";
    std::cout << separateVector(categories, '\t') <<"\n";
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

std::string takeSingleInput(std::vector<std::string> categories, std::string promptString)
{
  bool process = false;
  std::string selected;
  while (!(process))
  {
    std::cout << promptString;
    selected = takeInput();
    process = checkInput(selected, categories);
    if (process == false)
    {
      std::cout << "Please check your input and try again. \n";
    }
  }
  return selected;
}

void makePickfile(std::vector<std::pair<std::string, std::string>> info)
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
  for (auto& item : info)
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


void pickfileSelector()
{
  // Assuming one folder that holds pickfiles for now, which is textfiles/SingleMassSnowmass
  fs::path dir("textfiles/SingleMassSnowmass");
  
  try
  {
    if (fs::exists(dir))
    {
      std::cout << dir << "Is a directory containing:\n";
      for (auto entry : fs::directory_iterator(dir))
      {
        std::cout << entry.path() << "\n";
      }
    }
  }
  catch(const fs::filesystem_error& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

// Reads processes.txt and returns a string with the names of the processes in the file
std::vector<std::string> processNames();

// Fills the idtypes of the selected process
std::vector<IDType> findProcessIDTypes(std::string process);

// Takes a vector of strings and outputs a string with the contents of the vector tab separated
std::string separateVector(std::vector<std::string> list, char separator);

// Checks weather the selected item was in the list 
bool checkInput(std::string input, std::vector<std::string> list);

// A function that gets the user's input, adds a newline character and converts the string to lowercase.
std::string takeInput();

// A function that prompts for the user's input, and returns a pair with the user's input and corresponding category
std::pair<std::string, std::string> promptInput(std::string name, std::vector<std::string> categories);

void makePickfile(std::map<std::string, std::string> info);

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


  std::cout << "Reading input file " << inputFile << std::endl;

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
    // Ask for process name
    std::pair<std::string, std::string> processName = promptInput("Process", processNames());
    std::map<std::string, std::string> pickfileInfo;
    pickfileInfo.insert(processName);

    // Ask for IDTypes
    std::vector<IDType> idtypes = findProcessIDTypes(processName.second);
    for (auto& idtype : idtypes)
    {
      std::pair<std::string, std::string> newCategory = promptInput(idtype.getName(), idtype.getCategories());
      pickfileInfo.insert(newCategory);
    }

    // Ask for the filename
    std::cout << "Enter a filename (with the .txt) and its path from the bin folder \n filename: bin/";
    std::string filename = takeInput();
    std::string anamethatiwouldliketotest = "filename";
    pickfileInfo.insert(anamethatiwouldliketotest, filename);
    // add something that checks if it is actually a text file

    // Print current status, ask for any changes
    std::cout << "Here is the current information entered: \n";
    std::vector<std::string> namesVector;
    for (const auto& item : pickfileInfo)
    {
      std::cout << item.first << ": " << item.second << "\n";
      namesVector.push_back(item.first);
    }

    std::cout << "Would you like to change anything? If no, the pickfile will be generated. \n (Y or N):";
    std::string change;
    std::cin >> change;
    if (change == "Y")
    {
      // Come back to this
    }
    else if (change == "N")
    {
      
    }
  }

  analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  return 0;
}


std::vector<std::string> processNames()
{
  std::vector<std::string> processes;
  std::ifstream textfile("processes.txt");
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
  std::ifstream textfile("processes.txt");
  std::string line;
  bool foundProcess = false;
  std::vector<std::string> IDCategories;

  // Reading processes.txt line by line
  while (getline(textfile, line))
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
        textfile.close();
        return idtypes;  
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
  while (processInput == false)
  {
    std::cout << "Please choose a" << name << " from the following: \n";
    std::cout << separateVector(categories, '\t') <<"\n\n";
    selected = takeInput();
    processInput = checkInput(selected, categories);
    if (processInput == false)
    {
      std::cout << '\'' << selected << "\' is not found in an entry from the list. Please try again. \n";
    }
  }
  std::pair<std::string, std::string> inputToReturn(name, selected);
  return inputToReturn;
}

void makePickfile(std::map<std::string, std::string> info)
{
  std::string name = info.at("filename");

  // create the textfile
  std::ofstream pickfile(name); 
  
  // find the process name and write it into the file
  name = info.at("Process");
  pickfile << name << '\n';

  // write each idtype into the file

  info.erase("filename");
  info.erase("Process");

  // Without filename and process, the map is now full of only idtypes
  std::ostringstream types;
  for (auto& item : info)
  {
    types << item.first << ": " << item.second << "\n";
  }

  pickfile << types.str() << "\n";
  pickfile.close();
}
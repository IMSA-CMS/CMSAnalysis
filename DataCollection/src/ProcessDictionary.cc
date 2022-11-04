#include "CMSAnalysis/DataCollection/interface/ProcessDictionary.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"
#include "CMSAnalysis/DataCollection/interface/FileParams.hh"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

void ProcessDictionary::loadProcesses(std::string filename) 
{
  std::string line;
  std::ifstream   textfile(filename);
  std::string processName;
  std::unordered_map<std::string, std::vector<std::string>> idTypes;

  if (!textfile)
  {
    throw std::runtime_error("Cannot access textfile " + filename);
  }

  while (std::getline(textfile, line))  // splitting pickfiles by tab
  {
    std::vector<std::string> tokens; 
    if (line.empty()) 
    {
      continue; //Go to next line if the line is empty
    }
    std::istringstream iss(line);   
    std::string token; 
    while (std::getline(iss, token, '\t'))
    {
      tokens.push_back(token);  
      // std::cout << token << '\n';
    }
    if (tokens.size() == 1) 
    {
      if (!(processName.empty()))
      {
        Process newProcess(processName, idTypes);
        processes.push_back(newProcess);
      }
      processName = tokens[0];
      idTypes.clear();
      continue;
    }
    std::size_t found = line.find(":"); // if colon found
    std::string type = tokens[0].substr(0, found);
    tokens.erase(tokens.begin());
    //    IDType idType(type, tokens);
    idTypes.insert({type, tokens});
  }
  Process newProcess(processName, idTypes);
  processes.push_back(newProcess);
}

std::vector<FileParams> ProcessDictionary::readFile(std::string filename)
{
  std::string line;
  std::ifstream textfile(filename);
  std::string process;
  std::vector<std::string> values;
  std::unordered_map<std::string, std::vector<std::string>> idTypes; 
  std::vector<FileParams> fileParamsVector;
  std::vector<Process> pickfileProcesses;

  while (std::getline(textfile, line))  // splitting pickfiles by tab
  {
    if ((line.find("\t") == std::string::npos))  // if tab is not found
    {
      if (!process.empty())
      {
        Process newProcess(process, idTypes);
        pickfileProcesses.push_back(newProcess);
        idTypes.clear();
        process.clear();
      }
      process = line;
      continue; // continue if there is no space character, meaning it's the name of the process
    }
    else if (line.empty()) 
    {
      continue; // continue if it's just a blank line
    }
    else if (line.find(":") != std::string::npos) 
    // if there's a colon, everything before the colon is the "type."
    // everything after is turned into a vector called "values."
    {
      std::size_t found = line.find(":"); // if colon found
      std::string type = line.substr(0, found); // assign characters before : to type
      std::istringstream iss(line);
      std::string token;
      std::vector<std::string> tokens;
      std::getline(iss, token, '\t'); // getting rid of the first thing
      while (std::getline(iss, token, '\t'))
      {
        tokens.push_back(token);
      }
      idTypes.insert({type, tokens});
    }
  }
  if (!process.empty())
      {
        Process newProcess(process, idTypes);
        pickfileProcesses.push_back(newProcess);
      }
  std::vector<int> indices (idTypes.size(), 0);

  while(true)
  {
    auto newProcess = findProcess(process); // check function
    auto fileParams = getFileparams(newProcess, idTypes, indices); // Makes a file param with given data
    checkIDTypes(newProcess.getIDTypes(), idTypes); // check function
    fileParamsVector.push_back(fileParams); // adds the fileparams to a fileparams vector

    auto indexIterator = indices.begin();
    bool changeIndex = true;
    for(auto mapIterator = idTypes.begin(); mapIterator != idTypes.end(); ++mapIterator, ++indexIterator)
    {
      if(changeIndex)
      {
        ++(*indexIterator); // add 1 to whatever row you're on

        if(*indexIterator >= static_cast<int>(mapIterator->second.size())) // name is first, list is second. 
        {
          *indexIterator = 0;
          changeIndex = true;
        }
        else
        {
          changeIndex = false;
        }
        
      }
    }
    if(changeIndex)
    {
      break;
    }
  }
  return fileParamsVector;
}
// idTypes.
// idTypes.insert({type, tokens});
// Write code that assumes there's one for each. Search through vectors, find process, call getParams. 

Process& ProcessDictionary::findProcess(std::string newProcess) // fill in these functions
{
  for (auto& process : processes)
  {
    if (process.getName() == newProcess)
    {
      return process;
    }
  }
  // the process is not found in processes
  throw std::runtime_error("Cannot find process named " + newProcess + " in findProcess()!") ;
}

void ProcessDictionary::checkIDTypes(std::vector<IDType> idtypes, std::unordered_map<std::string, std::vector<std::string>> typesToCheck)
{
  for (auto& typeToCheck : typesToCheck)
  {
    bool checkTypeName = false;
    for (auto& idtype : idtypes)
    {
      if (idtype.getName() == typeToCheck.first)
      {
        checkTypeName = true;
        for (auto& categoryToCheck : typeToCheck.second)
        {
          bool checkCategory = false;
          for (auto& category : idtype.getCategories())
          {
            if (category == categoryToCheck)
            {
              checkCategory = true;
              break;
            }
          }
          if (checkCategory == false)
          {
            throw std::runtime_error("Category " + categoryToCheck + " is not found in the idtype "
            + idtype.getName() + " in processes!");
          }
        }
        break;
      }
    }
    if (checkTypeName == false)
    {
      throw std::runtime_error("IDType name " + typeToCheck.first + " is not found in processes.");
    }
  }
}

FileParams ProcessDictionary::makeFileparams(Process& processName, std::vector<std::pair<std::string, std::string>> pairs) 
{
  std::map<std::string, std::string> parameters;
  for (auto& pair : pairs)
  {
    parameters.insert(pair);
  }
  return FileParams(processName.getName(), parameters);
}

FileParams ProcessDictionary::getFileparams(Process& processName, std::unordered_map<std::string, std::vector<std::string>> types, std::vector<int> indices)
{
  std::vector<std::pair<std::string, std::string>> parameters;
  int counter = 0; // notes which "type" to use, e.g. "mass", "lepton", etc.
  for (auto& type : types) // loops through indices, e.g. [0, 1, 0]
  {
    std::string name = type.first; // gets the name from an idType
    std::vector<std::string> categories = type.second; // gets the values or "categories" from a type    
    std::string category = categories[indices[counter]]; // gets the category value as determined by the index
    std::pair<std::string, std::string> pair; // makes a pair out of name and value
    pair.first = name;
    pair.second = category;
    // std::cout << pair.first << ", " << pair.second << '\n';
    parameters.push_back(pair); // pushes pair to parameters vector
    counter++;
  }
  return makeFileparams(processName, parameters); // passes vector of parameters into makeFileparams
  // assignment 1/18:
 // right now, idTypes has a string that points to a vector of strings. 
 // e.g. lepton -> electron, muon
 // turn that into a vector<pair<string, string>>. "mass", "4" is one, "lepton," "muon" is another. 
 // pass vector into makeFileparams().
}

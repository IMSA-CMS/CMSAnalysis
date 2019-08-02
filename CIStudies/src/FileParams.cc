/*
 * This is the Source file for FileParams, which will be used to find files
 * Written by: Jay Reiter, Matt Hokinson, and John Woods
 * Last Edited: May 9th, 2018
 * Edits to merge fileLocator to FileParams by Kaushal Gumpula and Emily Springer
 * Last Edited: June 11th, 2018
 */
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using std::map;
using std::cout;
using std::string;
using std::vector;

void Year::addMaps()
{
  addValuesToMap({"2016", "2017"});
  addAlternate("16", "2016");
  addAlternate("17", "2017");
}

void Helicity::addMaps()
{
  addValuesToMap({"LL", "LR", "RL", "RR"});

  addAlternate("ll", "LL");
  addAlternate("lr", "LR");
  addAlternate("rl", "RL");
  addAlternate("rr", "RR");
}

void Interference::addMaps()
{
  addValuesToMap({"Constructive", "Destructive"});

  addAlternates({"constructive", "CONSTRUCTIVE", "CON", "Con", "con"}, "Constructive");
  addAlternates({"destructive", "DESTRUCTIVE", "DES", "Des", "des"}, "Destructive");
}

void MassRange::addMaps()
{
  addValuesToMap({"M300", "M800", "M1300", "M2000"});

  addAlternates({"300", "M300to800"}, "M300");
  addAlternates({"800", "M800to1300"}, "M800");
  addAlternates({"1300", "M1300to2000"}, "M1300");
  addAlternates({"2000", "M2000toInf"}, "M2000");
}

void Lambda::addMaps()
{
  vector<string> lambdaVals = {"1", "10", "16", "22", "24", "28", "32", "34", "40", "100k"};
  addValuesToMap(lambdaVals);

  for (const auto& val : lambdaVals)
    {
      if (val == "100k")
	{
	  addAlternates({"L100k", "100000000", "L100000000"}, val);
	}
      addAlternates({"L" + val, val + "000", "L" + val + "000"}, val);
    }
}

void Particle::addMaps()
{
  addValuesToMap({"Electron", "Muon"});
  addAlternates({"E", "EE", "e", "ee", "El", "Ele", "el", "ele", "electron"}, "Electron");
  addAlternates({"M", "Mu", "MM", "MuMu", "m", "mu", "mm", "mumu", "muon"}, "Muon");
}

FileParams::FileParams(const std::string& yr, const std::string& heli, const std::string& inter, 
		       const std::string& mrange, const std::string& L, const std::string& part) :
  year(yr),
  helicity(heli),
  interference(inter),
  massRange(mrange),
  lambda(L),
  particle(part)
{}


std::ostream& operator<<(std::ostream& stream, FileParams params)
{
    stream << "Your Parameters:\nYear: " << params.getYear()
	   << "\nHelicity: " << params.getHelicity() 
	   << "\nInterference: " << params.getInterference() 
	   << "\nMass Range: " << params.getMassRange() 
	   << "\nLambda: " << params.getLambda() 
	   << "\nParticle: " << params.getParticle() 
	   << '\n';
   
    return stream;
}


//creates a vector of root files through parsing through the text file given by the path
std::vector<std::string> FileParams::fileVector() const
{
  string textfilepath = locateTextFile();
  std::vector<std::string> rootfileVector;
  string line;
  std::ifstream myfile (textfilepath);
  if (myfile)
    {
      while(getline(myfile,line))
	{
	  rootfileVector.push_back(line);
	}
    }
  return rootfileVector;

}

//finds text file path
string FileParams::locateTextFile() const
{
  string yearString = getYear();
  string leptonString = getParticle() == "Electron" ? "E" : "Mu";
  string massString = yearString == "2017" ? massCutString2017() : getMassRange();
  string interferenceString = getInterference() == "Constructive" ? "Con" : "Des";

  string file = "textfiles/" + yearString + "/CITo2" + leptonString
    + massString + "_Lam" + getLambda() + interferenceString
    + getHelicity() + ".txt";

  std::cout << "Getting file " << file << std::endl;

  return file;
}

string FileParams::massCutString2017() const
{
  string mass = getMassRange();
  if (mass == "M300")
    return "M300to800";
  else if (mass == "M800")
    return "M800to1300";
  else if (mass == "M1300")
    return "M1300to2000";
  else if (mass == "M2000")
    return "M2000toInf";

  throw std::runtime_error("Invalid mass type!");
}

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

void Process::addMaps()
{
  addValuesToMap({"CI", "ADD", "ADD2", "DY", "Diboson", "top", "QCD"});

  addAlternates({"ci", "Ci", "cI"}, "CITo2");
  addAlternates({"add", "Add", "aDd", "adD", "aDD", "LED", "led"}, "ADD");
  addAlternates({"add2", "Add2", "aDd2", "adD2", "aDD2"}, "ADD2");
  addAlternates({"dy", "Drell Yan", "Drell-Yan"}, "DY");
  addAlternates({"diboson", "DB", "db"}, "Diboson");
  addAlternate("Top", "top");
  addAlternate("qcd", "QCD");
}

void Year::addMaps()
{
  addValuesToMap({"2016", "2017", "2018"});
  addAlternate("16", "2016");
  addAlternate("17", "2017");
  addAlternate("18", "2018");
}

void Helicity::addMaps()
{
  addValuesToMap({"LL", "LR", "RL", "RR"});

  vector<string> dibosons = {"WZ", "WZ3LNu", "WZ2L2Q", "ZZ", "ZZ2L2Nu", "ZZ2L2Q", "ZZ4L", "WW"};
  vector<string> tops = {"ttbar", "tW", "Wantitop"};
  vector<string> qcd = {"Wjets"};

  addValuesToMap(dibosons);
  addValuesToMap(tops);
  addValuesToMap(qcd);

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
  vector<string> massVals = {"M300", "M800", "M1300", "M1700", "M2000", "M120", "M200", "M400", "M800", "M1400", "M2300", "M3500", "M4500", "M6000", "M600", "M1200", "M2500", "M500", "M1200", "M1800"};
  addValuesToMap(massVals);
  addValuesToMap({""});
  

  addAlternates({"300", "M300to800"}, "M300");
  addAlternates({"800", "M800to1300"}, "M800");
  addAlternates({"1300", "M1300to2000"}, "M1300");
  addAlternate("1700", "M1700");
  addAlternates({"2000", "M2000toInf"}, "M2000");

  for (const auto& val : massVals)
    {
      addAlternate(val.substr(1), val);
    }
}

void Lambda::addMaps()
{
  vector<string> lambdaVals = {"1", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "10", "16", "22", "24", "28", "32", "34", "40", "100k"};
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
  addValuesToMap({"Electron", "Muon", "Both"});
  addAlternates({"E", "EE", "e", "ee", "El", "Ele", "el", "ele", "electron"}, "Electron");
  addAlternates({"M", "Mu", "MM", "MuMu", "m", "mu", "mm", "mumu", "muon"}, "Muon");
  addAlternate("both", "Both");
}

FileParams::FileParams(const std::string& proc, const std::string& yr, const std::string& heli, const std::string& inter, 
		       const std::string& mrange, const std::string& L, const std::string& part) :
  process(proc),
  year(yr),
  helicity(heli),
  interference(inter),
  massRange(mrange),
  lambda(L),
  particle(part)
{}

// Default values - notice how dangerous this is
FileParams::FileParams() :
  process("CI"),
  year("2016"),
  helicity("LL"),
  interference("Constructive"),
  massRange("M300"),
  lambda("16"),
  particle("Electron")
{}

std::vector<std::string> FileParams::fileVector() const
{
  string textFilePath = locateTextFile();
  std::cout << "Processing file " << textFilePath << std::endl;

  std::vector<std::string> rootfileVector;
  string line;
  std::ifstream myfile (textFilePath);
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
  string processString;
  if (getProcess() == Process::CI())
    processString = "CITo2";
  else if (getProcess() == Process::ADD())
    processString = "ADDGravToLL";
  else 
    processString = getProcess();

  string yearString = getYear();
  string leptonString = getParticle() == Particle::electron() ? "E" : "Mu";
  string massString;
  string interferenceString = getInterference() == Interference::constructive() ? "Con" : "Des";
  string helicityString = getHelicity();
  string lambdaString = "_Lam" + getLambda();


  if ((yearString == "2017" || yearString == "2018") && (processString == Process::CI() + "To2" || processString == Process::ADD() + "GravToLL" || processString == Process::ADD2() + "GravToLL"))
    massString = massCutString20172018(); 

  else if (processString == Process::DY())
    {
      massString = massCutStringDY();
      interferenceString = "";
      helicityString = "";
      lambdaString = "";
    }
  else
    massString = getMassRange();

  if (processString == "ADDGravToLL"||processString == "ADD2GravToLL")
    {
      leptonString = "";
      interferenceString = "";
      helicityString = "";
    }

  if (processString == Process::Diboson() || processString == Process::top() || processString == Process::QCD())
    {
      leptonString = "";
      interferenceString = "";
      massString = "";
      lambdaString = "";
      processString = "";

      if (helicityString == "WW" || helicityString == "ttbar")
	{
	  massString = massCutStringBackgrounds();
	}
    }
      

  string file = "textfiles/" + yearString + "/" + processString + leptonString
    + massString + lambdaString + interferenceString
    + helicityString + ".txt";

  std::cout << file;
  return file;
}

string FileParams::massCutString20172018() const
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

string FileParams::massCutStringDY() const
{
  string mass = getMassRange();
  if (mass == "M120")
    return "M120to200";
  else if (mass == "M200")
    return "M200to400";
  else if (mass == "M400")
    return "M400to800";
  else if (mass == "M800")
    return "M800to1400";
  else if (mass == "M1400")
    return "M1400to2300";
  else if (mass == "M2300")
    return "M2300to3500";
  else if (mass == "M3500")
    return "M3500to4500";
  else if (mass == "M4500")
    return "M4500to6000";
  else if (mass == "M6000")
    return "M6000toInf";

  throw std::runtime_error(mass + " Invalid mass type!");
}

string FileParams::massCutStringBackgrounds() const
{
  string mass = getMassRange();
  if (mass == "M500")
    return "M500to800";
  else if (mass == "M800")
    return "M800to1200";
  else if (mass == "M1200")
    return "M1200to1800";
  else if (mass == "M1800")
    return "M1800toInf";

  if (mass == "M200")
    return "M200to600";
  else if (mass == "M600")
    return "M600to1200";
  else if (mass == "M1200")
    return "M1200to2500";
  else if (mass == "M2500")
    return "M2500toInf";

  throw std::runtime_error(mass + "Invalid mass type!");
}

std::vector<std::string> FileParams::getAllValues() const
{
  std::vector<std::string> values = {process.getValue(), year.getValue(), helicity.getValue(), interference.getValue(), massRange.getValue(), lambda.getValue(), particle.getValue()};
  return values;
}

std::ostream& operator<<(std::ostream& stream, const FileParams& params)
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

bool operator==(const FileParams& p1, const FileParams& p2)
{
  return p1.getProcess() == p2.getProcess()
    && p1.getYear() == p2.getYear()
    && p1.getHelicity() == p2.getHelicity()
    && p1.getInterference() == p2.getInterference()
    && p1.getMassRange() == p2.getMassRange()
    && p1.getLambda() == p2.getLambda()
    && p1.getParticle() == p2.getParticle();
}

bool operator!=(const FileParams& p1, const FileParams& p2)
{
  return !(p1 == p2);
}

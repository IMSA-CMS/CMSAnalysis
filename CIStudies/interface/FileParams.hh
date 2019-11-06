#ifndef FILEPARAMS_HH
#define FILEPARAMS_HH

#include <string>
#include <iostream>
#include <map>

#include "IDType.hh"

class Process : public IDType
{
public:
  Process(const std::string& process) {setValue(process);}
  virtual std::string getTypeName() const override {return "Process";}

  static std::string CI() {return "CI";}
  static std::string ADD() {return "ADD";}
  static std::string DY() {return "DY";}
  static std::string Diboson() {return "Diboson";}
  static std::string top() {return "top";}
  static std::string QCD() {return "QCD";}

protected:
  virtual void addMaps() override;
};

// First, the different types (similar to enums) for different things to add

// The MC generation year (e.g., 2016)
class Year : public IDType
{
public:
  Year(const std::string& year) {setValue(year);}
  virtual std::string getTypeName() const override {return "Year";}
  
protected:
  virtual void addMaps() override;
};

// The helicity model for CI interactions (e.g., LL)
class Helicity : public IDType
{
public:
  Helicity(const std::string& helicity) {setValue(helicity);}
  virtual std::string getTypeName() const override {return "Helicity";}

  // Simple functions to return the right strings, so you don't have to remember conventions
  static std::string LL() {return "LL";}
  static std::string LR() {return "LR";}
  static std::string RL() {return "RL";}
  static std::string RR() {return "RR";}
  
protected:
  virtual void addMaps() override;
};

// The interference model for CI interactions
class Interference : public IDType
{
public:
  Interference(const std::string& interference) {setValue(interference);}
  virtual std::string getTypeName() const override {return "Interference";}

  // Simple functions to return the right strings, so you don't have to remember conventions
  static std::string constructive() {return "Constructive";}
  static std::string destructive() {return "Destructive";}
  
protected:
  virtual void addMaps() override;
};

// The lower mass cut used in the generation of the file (e.g., M300)
class MassRange : public IDType
{
public:
  MassRange(const std::string& massRange) {setValue(massRange);}
  virtual std::string getTypeName() const override {return "MassRange";}
  
protected:
  virtual void addMaps() override;
};

// The value of lambda for CI and ADD events, in TeV (e.g., 16)
class Lambda : public IDType
{
public:
  Lambda(const std::string& lambda) {setValue(lambda);}
  virtual std::string getTypeName() const override {return "Lambda";}
  
protected:
  virtual void addMaps() override;
};

// The lepton type (e.g., Electron)
class Particle : public IDType
{
public:
  Particle(const std::string& particle) {setValue(particle);}
  virtual std::string getTypeName() const override {return "Particle";}

  // Simple functions to return the right strings, so you don't have to remember conventions
  static std::string electron() {return "Electron";}
  static std::string muon() {return "Muon";}
  static std::string both() {return "Both";}
  
protected:
  virtual void addMaps() override;
};

class FileParams
{  
public: 

  FileParams(const std::string& proc, const std::string& yr, const std::string& heli, const std::string& inter, const std::string& mrange,
	     const std::string& L, const std::string& part);
  // Default constructor - use with care because it assigns default values!
  FileParams();

  //getters
  std::string getProcess() const { return process.getValue(); }
  std::string getYear() const { return year.getValue(); }
  std::string getHelicity() const { return helicity.getValue(); }
  std::string getInterference() const { return interference.getValue(); }
  std::string getMassRange() const { return massRange.getValue(); }
  std::string getLambda() const { return lambda.getValue(); }
  std::string getParticle() const { return particle.getValue(); }

  //get-all
  std::vector<std::string> getAllValues() const;

  // Returns a vector of all filenames for this set of parameters
  std::vector<std::string> fileVector() const;
 
  // Returns the name of the text file that contains the file
  std::string locateTextFile() const;

  // A simple function to adjust mass cut naming conventions for 2017
  // Public just in case someone else needs it
  std::string massCutString20172018() const;
  std::string massCutStringDY() const;
  std::string massCutStringBackgrounds() const;


private:
  Process process;
  Year year;
  Helicity helicity;
  Interference interference;
  MassRange massRange;
  Lambda lambda;
  Particle particle;
};

// output and equality operators
std::ostream& operator<<(std::ostream& stream, const FileParams& params);
bool operator==(const FileParams& p1, const FileParams& p2);
bool operator!=(const FileParams& p1, const FileParams& p2);

#endif

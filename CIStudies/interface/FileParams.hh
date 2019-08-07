#ifndef FILEPARAMS_HH
#define FILEPARAMS_HH

/*
 * This is a header file for the fileParams struct
 * Written by Jay Reiter, John Woods, and Matt Hokinson
 * Last Edited: May 9th, 2018
 * Edits to merge fileLocator to FileParams by Kaushal Gumpula and Emily Springer
 * Last Edited: June 11th, 2018
 */

#include <string>
#include <iostream>
#include <map>

#include "IDType.hh"

class Year : public IDType
{
public:
  Year(const std::string& year) {setValue(year);}
  virtual std::string getTypeName() const override {return "Year";}
  
protected:
  virtual void addMaps() override;
};

class Helicity : public IDType
{
public:
  Helicity(const std::string& helicity) {setValue(helicity);}
  virtual std::string getTypeName() const override {return "Helicity";}

  static std::string LL() {return "LL";}
  static std::string LR() {return "LR";}
  static std::string RL() {return "RL";}
  static std::string RR() {return "RR";}
  
protected:
  virtual void addMaps() override;
};

class Interference : public IDType
{
public:
  Interference(const std::string& interference) {setValue(interference);}
  virtual std::string getTypeName() const override {return "Interference";}

  static std::string constructive() {return "Constructive";}
  static std::string destructive() {return "Destructive";}
  
protected:
  virtual void addMaps() override;
};

class MassRange : public IDType
{
public:
  MassRange(const std::string& massRange) {setValue(massRange);}
  virtual std::string getTypeName() const override {return "MassRange";}
  
protected:
  virtual void addMaps() override;
};

class Lambda : public IDType
{
public:
  Lambda(const std::string& lambda) {setValue(lambda);}
  virtual std::string getTypeName() const override {return "Lambda";}
  
protected:
  virtual void addMaps() override;
};

class Particle : public IDType
{
public:
  Particle(const std::string& particle) {setValue(particle);}
  virtual std::string getTypeName() const override {return "Particle";}

  static std::string electron() {return "Electron";}
  static std::string muon() {return "Muon";}
  
protected:
  virtual void addMaps() override;
};

class FileParams
{  
public: 

  FileParams(const std::string& yr, const std::string& heli, const std::string& inter, const std::string& mrange,
	     const std::string& L, const std::string& part);
  // Default constructor - use with care
  FileParams();

  //getters
  std::string getYear() const { return year.getValue(); }
  std::string getHelicity() const { return helicity.getValue(); }
  std::string getInterference() const { return interference.getValue(); }
  std::string getMassRange() const { return massRange.getValue(); }
  std::string getLambda() const { return lambda.getValue(); }
  std::string getParticle() const { return particle.getValue(); }

  //function for parsing and locating textfiles
  std::vector<std::string> fileVector() const;
  std::string locateTextFile() const;

  std::string massCutString2017() const;

private:
  //file parameters
  Year year;
  Helicity helicity;
  Interference interference;
  MassRange massRange;
  Lambda lambda;
  Particle particle;
};

std::ostream& operator<<(std::ostream& stream, const FileParams& params);
bool operator==(const FileParams& p1, const FileParams& p2);
bool operator!=(const FileParams& p1, const FileParams& p2);

#endif

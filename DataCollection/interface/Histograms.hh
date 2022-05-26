#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "GenSimRecoPrototype.hh"
#include "ResolutionPrototype.hh"

// This file was created by Liam Nelson, IMSA class of 2022.
// Last updated 05/17/2022
// 
// This is a unified file containing all of our simplest histograms to
// reduce the amount of files in our codebase. Histogram classes with
// constructors are left out.

class AllLeptonInvariantMassHist : public GenSimRecoPrototype {
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.calculateAllLeptonInvariantMass();
    return {inv};
  }
};

class InvariantMassHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.getInvariantMass();
    return {inv};
  }
};

class MassResolutionHist : public ResolutionPrototype
{
  using ResolutionPrototype::ResolutionPrototype;

  double getValue(const ParticleCollection& partColl) const
  {
    return partColl.getInvariantMass();
  }
};

class OppositeSignInvariantMassHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.calculateOppositeSignInvariantMass();
    return {inv};
  }
};

class PtHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto Pt = particles.getLeadingTransverseMomentum();
    return {Pt};
  }
};

class PtResolutionHist : public ResolutionPrototype
{
  using ResolutionPrototype::ResolutionPrototype;

  double getValue(const ParticleCollection& partColl) const
  {
    //std::cerr << "congrats you made it" << std::endl;
    return partColl.getLeadingTransverseMomentum();
  }
};

class ThirdMuonPtHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto Pt = particles.getNthHighestPt(3);
    return {Pt};
  }
};

#endif

#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "GenSimRecoPrototype.hh"
#include "ResolutionPrototype.hh"
#include "LeptonJetMLCalculator.hh"
#include "LeptonJetReconstructionModule.hh"

// This file was created by Liam Nelson, IMSA class of 2022.
// Last updated 05/17/2022
//
// This is a unified file containing all of our simplest histograms to
// reduce the amount of files in our codebase. Histogram classes with
// constructors are left out.

class AllLeptonInvariantMassHist : public GenSimRecoPrototype
{
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

  virtual double getValue(const ParticleCollection<Lepton> &partColl) const
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

  virtual double getValue(const ParticleCollection<Lepton> &partColl) const
  {
    // std::cerr << "congrats you made it" << std::endl;
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

// write histogram for Gamma distribution

class GammaHistogram : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getParticles(level, ParticleType::darkPhoton());
    std::vector<double> outputs;
    for (const auto& particle : particles)
    {
      double gammaVal = particle.getFourVector().Gamma();
      outputs.push_back(gammaVal);
    }
    return outputs;
  }
  
    
  };



class PileUpHist : public HistogramPrototype1D
{
  using HistogramPrototype1D::HistogramPrototype1D;

  std::vector<double> value() const
  {
    double pileUp = getInput()->getNumPileUpInteractions();
    return {pileUp};
  }
};

class LeptonJetMLHist : public GenSimRecoPrototype
{
public:
  LeptonJetMLHist(InputModule::RecoLevel type, const std::string &iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetMLCalculator> iMlCalc, std::shared_ptr<LeptonJetReconstructionModule> iRecoCalc) : GenSimRecoPrototype(type, iname, iNBins, iminimum, imaximum),
                                                                                                                                                                                                                                   mLCalc(iMlCalc),
                                                                                                                                                                                                                                   recoCalc(iRecoCalc)
  {
  }

protected:
  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto jets = recoCalc->getLeptonJets();
    std::vector<double> outputs;
    for (LeptonJet jet : jets)
    {
      outputs.push_back(mLCalc->CalculateMLValue(jet));
    }
    return outputs;
  }

private:
  std::shared_ptr<LeptonJetMLCalculator> mLCalc;
  std::shared_ptr<LeptonJetReconstructionModule> recoCalc;
};


class IsolationHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    std::vector<double> isolation;
    for(auto particle : particles.getParticles()) {
        if(particle.getIsolation() != 0) {
	    isolation.push_back(particle.getIsolation());
    	}
    }
    return isolation;
  }
};

#endif

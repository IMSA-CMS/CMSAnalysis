#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include <iostream>

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/Histograms/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/Modules/interface/MLCalculator.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"

// This file was created by Liam Nelson, IMSA class of 2022.
// Last updated 05/17/2022
//
// This is a unified file containing all of our simplest histograms to
// reduce the amount of files in our codebase. Histogram classes with
// constructors are left out.

class AllLeptonInvariantMassHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.calculateAllLeptonInvariantMass();
    return {inv};
  }
};

class InvariantMassHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.getInvariantMass();
    return {inv};
  }
};

class LeptonJetInvariantMassHist : public HistogramPrototype1D
{
  using HistogramPrototype1D::HistogramPrototype1D;
  std::vector<double> value() const
  {
    std::vector<double> invariantMass;
      auto particles = getInput()->getSpecial("leptonJet");
      for (auto particle: particles)
      {
        LeptonJet lj(particle);
        auto inv = lj.getMass();
        invariantMass.push_back(inv);
      }
      return invariantMass;
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

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = particles.calculateOppositeSignInvariantMass();
    return {inv};
  }
};

class PtHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
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

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
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

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
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
  LeptonJetMLHist(EventInput::RecoLevel type, const std::string &iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<MLCalculator> iMlCalc, std::shared_ptr<LeptonJetReconstructionModule> iRecoCalc) : GenSimRecoPrototype(type, iname, iNBins, iminimum, imaximum),
                                                                                                                                                                                                                                   mLCalc(iMlCalc),
                                                                                                                                                                                                                                   recoCalc(iRecoCalc)
  {
  }

protected:
  std::vector<double> protectedValue(EventInput::RecoLevel level) const
  {
    auto jets = recoCalc->getLeptonJets();
    std::vector<double> outputs;
    for (LeptonJet jet : jets)
    {
      ParticleCollection<Particle> collection;
      collection.addParticle(jet);
      outputs.push_back(mLCalc->getOutput(collection));
      //std::cout << mLCalc->getOutput(jet);
      //print here
      //look at the prior fucntion
    }
    
    return outputs;
  }

private:
  std::shared_ptr<MLCalculator> mLCalc;
  std::shared_ptr<LeptonJetReconstructionModule> recoCalc;
};


class IsolationHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
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

class DxyHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;
  public:
    DxyHist(EventInput::RecoLevel type, const std::string &iname, int iNBins, double iminimum, double imaximum) : 
      GenSimRecoPrototype(type, iname, iNBins, iminimum, imaximum) {}
  protected:
    std::vector<double> protectedValue(EventInput::RecoLevel level) const
    {
      auto particles = getInput()->getLeptons(level);
      std::vector<double> dxy;
      for(auto particle : particles.getParticles()) {
        double particleDxy = particle.getDXY();
        dxy.push_back(particleDxy);
      }
      return dxy;
    }
};

class DzHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;
  public:
    DzHist(EventInput::RecoLevel type, const std::string &iname, int iNBins, double iminimum, double imaximum) : 
      GenSimRecoPrototype(type, iname, iNBins, iminimum, imaximum) {}

  protected:
    std::vector<double> protectedValue(EventInput::RecoLevel level) const
    {
      auto particles = getInput()->getLeptons(level);
      std::vector<double> dz;
      for(auto particle : particles.getParticles()) {
        double particleDz = particle.getDZ();
        dz.push_back(particleDz);
      }
      return dz;
    }
};

// plots highest lepton-photon invariant mass in the event
class PhotonInvariantMassHist : public GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(EventInput::RecoLevel level) const
  {
    auto particles = getInput()->getParticles(level);
    bool ePlusGamma = false;
    double highInvMass = 0;
    double thisInvMass = 0;

    for(auto particle : particles.getParticles()) {
      for (auto particle2 : particles.getParticles())
      {
        ePlusGamma = (particle.getType() == ParticleType::electron() && particle2.getType() == ParticleType::photon() ) || (particle2.getType() == ParticleType::electron() && particle.getType() == ParticleType::photon());
        thisInvMass = (particle.getFourVector() + particle2.getFourVector()).M();

        if (ePlusGamma && (thisInvMass > highInvMass)) {
          highInvMass = thisInvMass;
        }
      }
    }
    return {highInvMass};
  }
};


#endif

#include "CMSAnalysis/Histograms/interface/ScaledMassHist.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "TLorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"

#include <cmath>
#include <iostream>
#include <string>

ScaledMassHist::ScaledMassHist(const MatchingModule& matchingModule, const std::string& iname, int nBins, int minMass, int maxMass, bool scaledUp, bool scaledUpDown, bool typeSwitch) :
  HistogramPrototype1D(iname, nBins, minMass, maxMass),
  matching(matchingModule),
  scaleUp(scaledUp),
  scaleUpDown(scaledUpDown),
  isGenSim(typeSwitch)

{}

std::vector<double> ScaledMassHist::value() const
{
  auto bestPairs = matching.getMatchingBestPairs();

  // const ParticleCollection& genParticles;
  // const ParticleCollection& recoParticles;

  if (bestPairs.getSize() == 2)
    {
      const ParticleCollection<GenSimParticle>& genParticles = bestPairs.getGenParticles();
      const ParticleCollection<Particle>& recoParticles = bestPairs.getRecoParticles();
      // genParticles = bestPairs.getGenParticles();
      // recoParticles = bestPairs.getRecoParticles();
      // double eventWeight = 1.000;
      
      if (isGenSim)
  	{
    	  return {getScaledInvariantMass(genParticles, scaleUp, scaleUpDown)};
  	}

      else
  	{
          return {getScaledInvariantMass(recoParticles, scaleUp, scaleUpDown)};
        }
    }

    else
    {
      return {0};
    }
}


double ScaledMassHist::getScaledInvariantMass(const ParticleCollection<Particle>& bestParticles, bool scaleUp, bool scaleUpDown) const
{
  auto particles = bestParticles.getParticles();
  const double kappa = 0.00005;
  if (!particles.empty())
  {
    if (scaleUpDown == true)
    {
      double scaledPt1 = changePtUp(particles[0].getPt(), kappa);
      double scaledPt2 = changePtDown(particles[1].getPt(), kappa);
      return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
    }
    
    else
    {
      if (scaleUp == true)
      {
        double scaledPt1 = changePtUp(particles[0].getPt(), kappa);
        double scaledPt2 = changePtUp(particles[1].getPt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
      
      else
      {
        double scaledPt1 = changePtDown(particles[0].getPt(), kappa);
        double scaledPt2 = changePtDown(particles[1].getPt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
    }
  }

  else
  {
    return -1.000;
  }
  
}

/*double ScaledMassHist::getScaledInvariantMass(const ParticleCollection<GenSimParticle>& bestParticles, bool scaleUp, bool scaleUpDown) const
//the exact same as the ParticleCollection<Particle> version
{
  auto particles = bestParticles.getParticles();
  const double kappa = 0.00005;
  if (!particles.empty())
  {
    if (scaleUpDown == true)
    {
      double scaledPt1 = changePtUp(particles[0].getPt(), kappa);
      double scaledPt2 = changePtDown(particles[1].getPt(), kappa);
      return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
    }
    
    else
    {
      if (scaleUp == true)
      {
        double scaledPt1 = changePtUp(particles[0].getPt(), kappa);
        double scaledPt2 = changePtUp(particles[1].getPt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
      
      else
      {
        double scaledPt1 = changePtDown(particles[0].getPt(), kappa);
        double scaledPt2 = changePtDown(particles[1].getPt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
    }
  }

  else
  {
    return -1.000;
  }
  
}*/

double ScaledMassHist::calculateScaledInvariantMass(const Particle particle1, const Particle particle2, double scaledPt1, double scaledPt2) const
{
  double product = 2 * scaledPt1 * scaledPt2; 
  double diff = cosh(particle1.getEta() - particle2.getEta()) - cos(particle1.getPhi() - particle2.getPhi()); 
  double invariantMass = product * diff; 
  if (invariantMass > 0)
    {
      return sqrt(invariantMass);
    }
  else
    {
      return 0; 
    }
}


  

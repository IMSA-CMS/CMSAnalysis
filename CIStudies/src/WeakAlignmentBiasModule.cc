/*
#include "CIAnalysis/CIStudies/interface/WeakAlignmentBiasModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "TLorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <cmath>
#include <iostream>
#include <string>

WeakAlignmentBiasModule::WeakAlignmentBiasModule(const MatchingModule& matchingModule, int minMass, int maxMass, int massInterval) :
  matching(matchingModule),
  minMassCut(minMass),
  maxMassCut(maxMass),
  interval(massInterval)
{
}

void WeakAlignmentBiasModule::initialize()
{
  // const int numberOfBins = (maxMassCut - minMassCut) / interval;


  // for(int i = 0; i < numberOfBins; ++i)
    // {
      // const std::string migrationStr = "Migration";
      // makeHistogram(("genSimHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval, 
		    // ((i - 2) * interval + minMassCut), ((i + 3) * interval + minMassCut));
      // makeHistogram(("recoHistBin" + std::to_string(i * interval + minMassCut)), migrationStr, interval,
		    // ((i - 2) * interval + minMassCut), ((i + 3) * interval + minMassCut));
    // }
  
  const int numBins = (maxMassCut - minMassCut) / interval;
  makeHistogram("Weak Alignment Bias (Default 1)", "GenSim Relative Difference Default", numBins, minMassCut, maxMassCut);
  makeHistogram("Weak Alignment Bias (Default 2)", "Reco Relative Difference Default", numBins, minMassCut, maxMassCut);

  makeHistogram("Weak Alignment Bias (Scaled Up 1)", "GenSim Relative Difference Scaled Up", numBins, minMassCut, maxMassCut);
  makeHistogram("Weak Alignment Bias (Scaled Up 2)", "Reco Relative Difference Scaled Up", numBins, minMassCut, maxMassCut);

  makeHistogram("Weak Alignment Bias (Scaled Down 1)", "GenSim Relative Difference Scaled Down", numBins, minMassCut, maxMassCut);
  makeHistogram("Weak Alignment Bias (Scaled Down 2)", "Reco Relative Difference Scaled Down", numBins, minMassCut, maxMassCut);

  makeHistogram("Weak Alignment Bias (Up Down 1)", "GenSim Relative Difference Mu Scaled Up AntiMu Scaled Down", numBins, minMassCut, maxMassCut);
  makeHistogram("Weak Alignment Bias (Up Down 2)", "Reco Relative Difference Mu Scaled Up AntiMu Scaled Down", numBins, minMassCut, maxMassCut);
   
}

bool WeakAlignmentBiasModule::process(const edm::EventBase& event)
{
  auto bestPairs = matching.getMatchingBestPairs();

  if (bestPairs.getSize() == 2)
    {
      const ParticleCollection& genParticles = bestPairs.getGenParticles();
      const ParticleCollection& recoParticles = bestPairs.getRecoParticles();
      double eventWeight = 1.000;

      double genSimMass = genParticles.getInvariantMass();
      double recoMass = recoParticles.getInvariantMass();
      fillHistogram("Weak Alignment Bias (Default 1)", genSimMass, eventWeight);
      fillHistogram("Weak Alignment Bias (Default 2)", recoMass, eventWeight);


      bool scaleUp = true;
      bool scaledUpDown = false;
      double genSimMassScaledUp = getScaledInvariantMass(genParticles, scaleUp, scaledUpDown);
      // std::cout << "Scaled Up Invariant Mass GenSim: " << genSimMassScaledUp << "\n";
      double recoMassScaledUp = getScaledInvariantMass(recoParticles, scaleUp, scaledUpDown);
      // std::cout << "Scaled Up Invariant Mass Reco: " << recoMassScaledUp << "\n";

      // double genSimRelDiffUp = calculateRelativeDifference(genSimMassScaledUp, genSimMass);
      // std::cout << "Scaled Up Relative Difference GenSim: " << genSimRelDiffUp << "\n";
      // double recoRelDiffUp = calculateRelativeDifference(recoMassScaledUp, recoMass);
      // std::cout << "Scaled Up Relative Difference Reco: " << recoRelDiffUp << "\n";

      fillHistogram("Weak Alignment Bias (Scaled Up 1)", genSimMassScaledUp, eventWeight);
      fillHistogram("Weak Alignment Bias (Scaled Up 2)", recoMassScaledUp, eventWeight);

      
      scaleUp = false;
      double genSimMassScaledDown = getScaledInvariantMass(genParticles, scaleUp, scaledUpDown);
      double recoMassScaledDown = getScaledInvariantMass(recoParticles, scaleUp, scaledUpDown);

      // double genSimRelDiffDown = calculateRelativeDifference(genSimMassScaledDown, genSimMass);
      // double recoRelDiffDown = calculateRelativeDifference(recoMassScaledDown, recoMass);

      fillHistogram("Weak Alignment Bias (Scaled Down 1)", genSimMassScaledDown, eventWeight);
      fillHistogram("Weak Alignment Bias (Scaled Down 2)", recoMassScaledDown, eventWeight);

      
      scaledUpDown = true;
      double genSimMassScaledUpDown = getScaledInvariantMass(genParticles, scaleUp, scaledUpDown);
      double recoMassScaledUpDown = getScaledInvariantMass(recoParticles, scaleUp, scaledUpDown);

      // double genSimRelDiffUpDown = calculateRelativeDifference(genSimMassScaledUpDown, genSimMass);
      // double recoRelDiffUpDown = calculateRelativeDifference(recoMassScaledUpDown, recoMass);

      fillHistogram("Weak Alignment Bias (Up Down 1)", genSimMassScaledUpDown, eventWeight);
      fillHistogram("Weak Alignment Bias (Up Down 2)", recoMassScaledUpDown, eventWeight);

      // std::string massBin = pickMassBin(genSimMass);
      // fillHistogram("genSimHistBin" + massBin, genSimMass);
      // fillHistogram("recoHistBin" + massBin, recoMass);
      
    }
  
  return true;
}

double WeakAlignmentBiasModule::getScaledInvariantMass(const ParticleCollection& bestParticles, bool scaleUp, bool scaleUpDown) const
{
  auto particles = bestParticles.getParticles();
  const double kappa = 0.00005;
  if (particles[0] && particles[1])
  {
    if (scaleUpDown == true)
    {
      double scaledPt1 = changePtUp(particles[0]->pt(), kappa);
      double scaledPt2 = changePtDown(particles[1]->pt(), kappa);
      return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
    }
    
    else
    {
      if (scaleUp == true)
      {
        double scaledPt1 = changePtUp(particles[0]->pt(), kappa);
        double scaledPt2 = changePtUp(particles[1]->pt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
      
      else
      {
        double scaledPt1 = changePtDown(particles[0]->pt(), kappa);
        double scaledPt2 = changePtDown(particles[1]->pt(), kappa);
        return calculateScaledInvariantMass(particles[0], particles[1], scaledPt1, scaledPt2);
      }
    }
  }

  else
  {
    return -1.000;
  }
  
}

double WeakAlignmentBiasModule::calculateScaledInvariantMass(const reco::Candidate* particle1, const reco::Candidate* particle2, double scaledPt1, double scaledPt2) const
{
  double product = 2 * scaledPt1 * scaledPt2; 
  double diff = cosh(particle1->eta() - particle2->eta()) - cos(particle1->phi() - particle2->phi()); 
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

std::string WeakAlignmentBiasModule::pickMassBin(double invariantMass)
{
  int mass = static_cast<int>(invariantMass) / interval * interval; //mass is invariantMass floored to the highest multiple of interval
  if (mass < minMassCut)
    {
      mass = minMassCut;
    }
  else if (mass > maxMassCut - interval)
    {
      mass = maxMassCut - interval;
    }

  return std::to_string(mass);
}
*/


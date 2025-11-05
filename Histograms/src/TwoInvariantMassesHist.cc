#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "TLorentzVector.h"

TwoInvariantMassesHist::TwoInvariantMassesHist(const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY){};

double calculateInvariantMass(Particle particle1, Particle particle2) 
{
  auto vec1 = particle1.getFourVector();
  auto vec2 = particle2.getFourVector();

  auto sum = vec1 + vec2;
  return sum.M();

}


// std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
// {
//   auto Leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);

//   if (Leptons.size() != 4) return {};

//   //Collection Initialize
//   ParticleCollection positiveLeptons;
//   ParticleCollection negativeLeptons;
//   ParticleCollection taus;

//   //Sorts Lepton Pairs
//   for (auto lepton : Leptons) {
//     if (lepton.getCharge() == 1) positiveLeptons.addParticle(lepton);
//     else if (lepton.getCharge() ==-1) negativeLeptons.addParticle(lepton);
//     else if (lepton.getCharge() == 0) taus.addParticle(lepton);
//   }

//   //if (positiveLeptons.size() != 2 || negativeLeptons.size() != 2) return {};

//   //Calculate Same Sign Invariant Masses
//   double positivePairInvariantMass = calculateInvariantMass(positiveLeptons[0],positiveLeptons[1]);
//   double negativePairInvariantMass = calculateInvariantMass(negativeLeptons[0],negativeLeptons[1]);

  //Pair Combos
  /* 
  eeee + -
  ee eu 
  ee uu
  eueu + -
  eu uu
  uuuu + -
  */

  /*
  
  eeee --> ee+ ee-
  eeuu --> ee uu
  uuuu --> uu+ uu-
  eueu --> eu+ eu-
  eeeu --> ee eu
  euuu --> eu uu
  uuuu --> uu+ uu-

  tau doesn't work because the charge is zero
  tau can split to fill in 2 of each

  1 tau:
    go with the one different charge 

  2 taus:
    try to balence charges, if not possible then choose the pair with the highest invarant mass
    if the other 2 particles are the same sign then the taus go together, otherwise check invarant mass for highest combination

  3 taus:
    2 taus and 1 tou with the other particle, check all combinations go with the highest invarant mass

  4 taus:
    same as 3 taus 

  order: flavor, charge, invarant mass

  1. check amount of taus
  2. split pairs based on taus
  3. check for order using flavor, charge, invarant mass


  

  */



std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
{
    auto Leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);
    if (Leptons.size() != 4) return {};
  auto bestPairing = findBestLeptonPairing();

  return {sortParticlePairs(bestPairing.first, bestPairing.second)};

}

std::pair<std::pair<Particle, Particle>, std::pair<Particle, Particle>> TwoInvariantMassesHist::findBestLeptonPairing() const
{
  auto Leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);

  // Collection Initialize
  ParticleCollection positiveLeptons;
  ParticleCollection negativeLeptons;
  ParticleCollection taus;

  // Sorts Leptons by charge
  for (auto lepton : Leptons) {
    if (lepton.getType() == ParticleType::tau()) {
      taus.addParticle(lepton);
    }
    else if (lepton.getCharge() == 1) {
      positiveLeptons.addParticle(lepton);
    }
    else if (lepton.getCharge() == -1) {
      negativeLeptons.addParticle(lepton);
    }
  }

  // Handle based on number of taus
  int tauCount = taus.size();
  
  // No taus - use the original logic for charged leptons
  if (tauCount == 0) 
  {
    
    double positivePairInvariantMass = calculateInvariantMass(positiveLeptons[0], positiveLeptons[1]);
    double negativePairInvariantMass = calculateInvariantMass(negativeLeptons[0], negativeLeptons[1]);
    
    int posElectronCount = 0;
    int negElectronCount = 0;

    if (positiveLeptons[0].getType() == ParticleType::electron()) ++posElectronCount;
    if (positiveLeptons[1].getType() == ParticleType::electron()) ++posElectronCount;
    if (negativeLeptons[0].getType() == ParticleType::electron()) ++negElectronCount;
    if (negativeLeptons[1].getType() == ParticleType::electron()) ++negElectronCount;

    if (posElectronCount == negElectronCount)
      return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    
    if (posElectronCount > negElectronCount) {
      return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    } 
    else {
      return {{negativeLeptons[0], negativeLeptons[1]}, {positiveLeptons[0], positiveLeptons[1]}};
    }
  }
  
  // 1 tau - pair with the lepton of different charge than the majority
  else if (tauCount == 1) {
    // Remaining leptons
    std::vector<Particle> otherLeptons;
    for (auto& lepton : Leptons) {
      if (lepton.getType() != ParticleType::tau()) {
        otherLeptons.push_back(lepton);
      }
    }

    
    // Count charges of other leptons
    int positiveCount = 0, negativeCount = 0;
    for (auto& lepton : otherLeptons) {
      if (lepton.getCharge() == 1) positiveCount++;
      else if (lepton.getCharge() == -1) negativeCount++;
    }
    
    // Pair tau with the minority charge lepton
    Particle tauPartner = Leptons[0]; // Placeholder
    ParticleCollection firstPair;
    ParticleCollection secondPair;
    
    if (positiveCount > negativeCount) {
      // Find the negative lepton to pair with tau
      for (auto& lepton : otherLeptons) {
        if (lepton.getCharge() == -1) {
          tauPartner = lepton;
          break;
        }
      }
      
      // Add tau and partner to first pair
      firstPair.addParticle(taus[0]);
      firstPair.addParticle(tauPartner);
      
      // Add remaining leptons to second pair
      for (auto& lepton : otherLeptons) {
        if (!(lepton == tauPartner)) {
          secondPair.addParticle(lepton);
        }
      }
    }
    else {
      // Find the positive lepton to pair with tau
      for (auto& lepton : otherLeptons) {
        if (lepton.getCharge() == 1) {
          tauPartner = lepton;
          break;
        }
      }
      
      // Add tau and partner to first pair
      firstPair.addParticle(taus[0]);
      firstPair.addParticle(tauPartner);
      
      // Add remaining leptons to second pair
      for (auto& lepton : otherLeptons) {
        if (!(lepton == tauPartner)) {
          secondPair.addParticle(lepton);
        }
      }
    }
    
    double firstInvariantMass = calculateInvariantMass(firstPair[0], firstPair[1]);
    double secondInvariantMass = calculateInvariantMass(secondPair[0], secondPair[1]);

    return {{firstPair[0], firstPair[1]}, {secondPair[0], secondPair[1]}};
  }
  
  // 2 taus - balance charges if possible, otherwise maximize invariant mass
  else if (tauCount == 2) {
    // Remaining leptons
    std::vector<Particle> otherLeptons;
    for (auto& lepton : Leptons) {
      if (lepton.getType() != ParticleType::tau()) {
        otherLeptons.push_back(lepton);
      }
    }
   
    // Check if other leptons have the same sign
    bool sameSign = (otherLeptons[0].getCharge() == otherLeptons[1].getCharge());
    
    if (sameSign) {
      // If other leptons have same sign, pair the taus together
      double tausPairMass = calculateInvariantMass(taus[0], taus[1]);
      double otherPairMass = calculateInvariantMass(otherLeptons[0], otherLeptons[1]);

      return {{taus[0], taus[1]}, {otherLeptons[0], otherLeptons[1]}};
    }
    else {
      // If other leptons have opposite sign, try all combinations and pick highest invariant mass
      double mass1 = calculateInvariantMass(taus[0], otherLeptons[0]) + 
                    calculateInvariantMass(taus[1], otherLeptons[1]);
                    
      double mass2 = calculateInvariantMass(taus[0], otherLeptons[1]) + 
                    calculateInvariantMass(taus[1], otherLeptons[0]);
      
      if (mass1 >= mass2) {
        double pair1Mass = calculateInvariantMass(taus[0], otherLeptons[0]);
        double pair2Mass = calculateInvariantMass(taus[1], otherLeptons[1]);
        return {{taus[0], otherLeptons[0]}, {taus[1], otherLeptons[1]}};
      }
      else {
        double pair1Mass = calculateInvariantMass(taus[0], otherLeptons[1]);
        double pair2Mass = calculateInvariantMass(taus[1], otherLeptons[0]);
        return {{taus[0], otherLeptons[1]}, {taus[1], otherLeptons[0]}};
      }
    }
  }
        // 3 taus - try all combinations and pick the one with highest invariant mass sum
      else if (tauCount == 3) {
        // Get the one non-tau lepton
        Particle otherLepton = Leptons[0]; // Placeholder
        for (auto& lepton : Leptons) {
          if (lepton.getType() != ParticleType::tau()) {
            otherLepton = lepton;
            break;
          }
        }
        
        // Try all possible combinations and pick the one with highest invariant mass
        double bestMassSum = 0.0;
        std::pair<std::pair<Particle, Particle>, std::pair<Particle, Particle>> bestParticles = {{Leptons[0], Leptons[0]}, {Leptons[0], Leptons[0]}}; // Placeholder

        // Try pairing each tau with the other lepton
        for (int i = 0; i < 3; i++) {
          ParticleCollection firstPair;
          ParticleCollection secondPair;
          
          // First pair: tau[i] + otherLepton
          firstPair.addParticle(taus[i]);
          firstPair.addParticle(otherLepton);
          
          // Second pair: remaining two taus
          for (int j = 0; j < 3; j++) {
            if (j != i) {
              secondPair.addParticle(taus[j]);
            }
          }
          
          double mass1 = calculateInvariantMass(firstPair[0], firstPair[1]);
          double mass2 = calculateInvariantMass(secondPair[0], secondPair[1]);
          double massSum = mass1 + mass2;
          
          if (massSum > bestMassSum) {
            bestMassSum = massSum;
            bestParticles = {{firstPair[0], firstPair[1]}, {secondPair[0], secondPair[1]}};
          }
        }
        
        return {bestParticles};
      }
      
      // 4 taus - try all combinations and pick the one with highest invariant mass sum
      else if (tauCount == 4) {
        // Try all possible pairings of the 4 taus (3 unique ways to pair them)
        double bestMassSum = 0.0;
        std::pair<double, double> bestMasses;
        
        // Pairing option 1: (0,1) and (2,3)
        double mass1_1 = calculateInvariantMass(taus[0], taus[1]);
        double mass1_2 = calculateInvariantMass(taus[2], taus[3]);
        double sum1 = mass1_1 + mass1_2;
        
        // Pairing option 2: (0,2) and (1,3)
        double mass2_1 = calculateInvariantMass(taus[0], taus[2]);
        double mass2_2 = calculateInvariantMass(taus[1], taus[3]);
        double sum2 = mass2_1 + mass2_2;
        
        // Pairing option 3: (0,3) and (1,2)
        double mass3_1 = calculateInvariantMass(taus[0], taus[3]);
        double mass3_2 = calculateInvariantMass(taus[1], taus[2]);
        double sum3 = mass3_1 + mass3_2;
        
        // Find the best pairing (highest invariant mass sum)
        if (sum1 >= sum2 && sum1 >= sum3) {
          return {{taus[0], taus[1]}, {taus[2], taus[3]}};
        } 
        else if (sum2 >= sum1 && sum2 >= sum3) {
          return {{taus[0], taus[2]}, {taus[1], taus[3]}};
        } 
        else {
          return {{taus[0], taus[3]}, {taus[1], taus[2]}};
        }
      }
      else{
        throw std::runtime_error("Unexpected number of taus in findBestLeptonPairing.");
      }
  }  


  // Sorts two particle pairs based on flavor, charge, and invariant mass
  // Returns the sorted pair of invariant masses in the correct order
  std::pair<double, double> TwoInvariantMassesHist::sortParticlePairs(const std::pair<Particle, Particle>& firstPair, const std::pair<Particle, Particle>& secondPair) const { 
    
    // invariant masses
    double firstMass = calculateInvariantMass(firstPair.first, firstPair.second);
    double secondMass = calculateInvariantMass(secondPair.first, secondPair.second);

// Check if one pair is the same flavors as the other pair
if (!((firstPair.first.getType() == secondPair.first.getType()) && firstPair.second.getType() == secondPair.second.getType())
 && !((firstPair.first.getType() == secondPair.second.getType()) && firstPair.second.getType() == secondPair.first.getType()))
    {
      // Count number of electrons and muons in each pair
      int nElectrons1 = 0;
      int nMuons1 = 0;

      if (firstPair.first.getType() == ParticleType::electron()) nElectrons1++;
      else if (firstPair.first.getType() == ParticleType::muon()) nMuons1++;
      if (firstPair.second.getType() == ParticleType::electron()) nElectrons1++;
      else if (firstPair.second.getType() == ParticleType::muon()) nMuons1++;


      
      int nElectrons2 = 0; 
      int nMuons2 = 0;
      if (secondPair.first.getType() == ParticleType::electron()) nElectrons2++;
      else if (secondPair.first.getType() == ParticleType::muon()) nMuons2++;
      if (secondPair.second.getType() == ParticleType::electron()) nElectrons2++;
      else if (secondPair.second.getType() == ParticleType::muon()) nMuons2++;
      

      // If one pair has more electrons or muons, return that pair first
      if (nElectrons1 != nElectrons2) {
        if (nElectrons1 > nElectrons2) {
          return {firstMass, secondMass};
        } else {
          return {secondMass, firstMass};
        }
      }
      if (nMuons1 != nMuons2) {
        if (nMuons1 > nMuons2) {
          return {firstMass, secondMass};
        } else {
          return {secondMass, firstMass};
        }
      }

      throw std::runtime_error("Unexpected case in sortParticlePairs: both pairs have same flavor composition but not identical.");
    }

    //   ParticleType firstType = firstPair[0].getType();
    //   ParticleType secondType = secondPair[0].getType();
      
    //   if (firstType != secondType) {
    //     // Sort by reconstruction efficiency: muon > electron > tau
    //     int firstRank = 0, secondRank = 0;
        
    //     if (firstType == ParticleType::muon()) firstRank = 1;
    //     else if (firstType == ParticleType::electron()) firstRank = 2;
    //     else if (firstType == ParticleType::tau()) firstRank = 3;
        
    //     if (secondType == ParticleType::muon()) secondRank = 1;
    //     else if (secondType == ParticleType::electron()) secondRank = 2;
    //     else if (secondType == ParticleType::tau()) secondRank = 3;
        
    //     if (firstRank < secondRank) {
    //       return {firstMass, secondMass};
    //     } else {
    //       return {secondMass, firstMass};
    //     }
    //   }
    // }

    // otherwise one pair has same flavor, other doesn't (tt vs em)
    // if (firstPairSameFlavor && !secondPairSameFlavor) {
    //   return {firstMass, secondMass};
    // }
    // if (!firstPairSameFlavor && secondPairSameFlavor) {
    //   return {secondMass, firstMass};
    // }
    
    // 2. Check charge sorting
    // Calculate total charge for each pair
    int firstPairCharge = firstPair.first.getCharge() + firstPair.second.getCharge();
    int secondPairCharge = secondPair.first.getCharge() + secondPair.second.getCharge();

    // If charges are different, return positive pair first
    if (firstPairCharge != secondPairCharge) {
      if (firstPairCharge > secondPairCharge) {
        return {firstMass, secondMass};
      } else {
        return {secondMass, firstMass};
      }
    }

    // 3. if not already sorted then sort by invariant mass
    if (firstMass <= secondMass) {
      return {firstMass, secondMass};
    } else {
      return {secondMass, firstMass};
    }
  }


  /* ------------previous code
  int posElectronCount = 0;
  int negElectronCount = 0;

  //this can probably be better optomized but i dont feel like dealing with it right now (kale 12/11/24)
  if (positiveLeptons[0].getType() == ParticleType::electron()) ++posElectronCount;
  if (positiveLeptons[1].getType() == ParticleType::electron()) ++posElectronCount;
  if (negativeLeptons[0].getType() == ParticleType::electron()) ++negElectronCount;
  if (negativeLeptons[1].getType() == ParticleType::electron()) ++negElectronCount;

  //Check if opposite sign particle pairs are the same leptons
  if (posElectronCount == negElectronCount)
    return {{positivePairInvariantMass,negativePairInvariantMass}};
  
  //Check which pair has more electrons if not equal
  if (posElectronCount > negElectronCount) {
    return {{positivePairInvariantMass,negativePairInvariantMass}};
  } 
  else 
  {
    return {{negativePairInvariantMass,positivePairInvariantMass}};
  }
 
  */
    



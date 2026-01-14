#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "TLorentzVector.h"
#include "CMSAnalysis/Utility/interface/Utility.hh"

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
    auto leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);
    if (leptons.size() != 4) return {};
  auto bestPairing = Utility::findBestLeptonPairing(leptons);

  return {sortParticlePairs(bestPairing.first, bestPairing.second)};

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
    



#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "TLorentzVector.h"

TwoInvariantMassesHist::TwoInvariantMassesHist(const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY){};

double calculateInvariantMass(Particle particle1, Particle particle2) {
  auto vec1 = particle1.getFourVector();
  auto vec2 = particle2.getFourVector();

  auto sum = vec1 + vec2;
  return sum.M();

}


std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
{
  auto Leptons = getInput()->getLeptons(EventInput::RecoLevel::Reco);

  if (Leptons.size() != 4) return {};

  //Collection Initialize
  ParticleCollection positiveLeptons;
  ParticleCollection negativeLeptons;

  //Sorts Lepton Pairs
  for (auto lepton : Leptons) {
    if (lepton.getCharge() == 1) positiveLeptons.addParticle(lepton);
    if (lepton.getCharge() ==-1) negativeLeptons.addParticle(lepton);
  }

  if (positiveLeptons.size() != 2 || negativeLeptons.size() != 2) return {};

  //Calculate Same Sign Invariant Masses
  double positivePairInvariantMass = calculateInvariantMass(positiveLeptons[0],positiveLeptons[1]);
  double negativePairInvariantMass = calculateInvariantMass(negativeLeptons[0],negativeLeptons[1]);

  //Pair Combos
  /* 
  eeee + -
  eeeu 
  eeuu
  eueu + -
  euuu
  uuuu + -
  */

  
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
 
    
 
}

#ifndef LEPTONJETRECONSTRUCTIONMODULE_HH
#define LEPTONJETRECONSTRUCTIONMODULE_HH

#include <vector>
#include "ProductionModule.hh"
#include "LeptonJet.hh"

class LeptonJetReconstructionModule : public ProductionModule
{
public:
  LeptonJetReconstructionModule(double deltaRCut = 0.5, double pTCut = 5);
  virtual bool process() override;
  const std::vector<LeptonJet>& getLeptonJets() const {return leptonJets;}
  const std::vector<double>& getDeltaRValues() const {return deltaRValues;}
  const std::vector<double>& getPtValues() const {return pTValues;}

private:
  LeptonJet createLeptonJet(Particle highestPtLepton) const;
  Particle findHighestPtLepton(std::vector<Particle> particles) const;
  void findDeltaRValues();
  void findPtValues();

  std::vector<LeptonJet> leptonJets;
  std::vector<double> deltaRValues;
  std::vector<double> pTValues;

  double DeltaRCut;
  double hadETCut;
  double pTCut;

};

#endif

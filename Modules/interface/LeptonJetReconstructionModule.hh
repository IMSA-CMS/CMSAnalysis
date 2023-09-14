#ifndef LEPTONJETRECONSTRUCTIONMODULE_HH
#define LEPTONJETRECONSTRUCTIONMODULE_HH

#include <vector>
#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

class LeptonJetReconstructionModule : public ProductionModule
{
public:
  LeptonJetReconstructionModule(double deltaRCut);
  virtual bool process() override;
  const std::vector<LeptonJet> &getLeptonJets() const { return leptonJets; }
  const std::vector<double> &getDeltaRValues() const { return deltaRValues; }
  const std::vector<double> &getPtValues() const { return pTValues; }
  const std::vector<LeptonJet> findLeptonJets(ParticleCollection<Lepton> recoCandidates);

private:
  LeptonJet createLeptonJet(Lepton highestPtLepton) const;
  Particle findHighestPtLepton(std::vector<Lepton> particles) const;
  void findDeltaRValues();
  void findPtValues();

  std::vector<LeptonJet> leptonJets;
  std::vector<double> deltaRValues;
  std::vector<double> pTValues;

  double DeltaRCut;
};

#endif

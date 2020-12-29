#ifndef LEPTONJETRECONSTRUCTIONMODULE_HH
#define LEPTONJETRECONSTRUCTIONMODULE_HH

#include <vector>
#include "ProductionModule.hh"
#include "LeptonJet.hh"

class RecoIdentificationModule;

class LeptonJetReconstructionModule : public ProductionModule
{
public:
  LeptonJetReconstructionModule(const RecoIdentificationModule& recoModule, double deltaRCut = 0.5);
  virtual bool process(const edm::EventBase& event) override;
  const std::vector<LeptonJet>& getLeptonJets() const {return leptonJets;}

private:  
  LeptonJet createLeptonJet(Particle highestPtLepton) const;
  Particle findHighestPtLepton(std::vector<Particle> particles) const;

  const RecoIdentificationModule& reco;
  std::vector<LeptonJet> leptonJets;
  double DeltaRCut;

};



#endif

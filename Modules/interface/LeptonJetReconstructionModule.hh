#ifndef LEPTONJETRECONSTRUCTIONMODULE_HH
#define LEPTONJETRECONSTRUCTIONMODULE_HH

#include <vector>
#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Histograms/interface/MLStripHist.hh"
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

  virtual std::string getName() override {return "LeptonJetReconstructionModule";}

  std::vector<double> getDeltaRValues() {return inputDeltaRValues;};
  std::vector<double> getDeltaPtValues() { return deltaPtValues; }
  std::vector<double> getLeadingPtValues() { return leadingPtValues; }
  std::vector<double> getSumPtValues() { return sumPtValues; }
  std::vector<double> getNParticlesValues() { return nParticlesValues; }
  std::vector<double> getEtaValues() { return etaValues; }
  std::vector<double> getMaxIsolationValues() { return maxIsolationValues; }

	std::shared_ptr<HistogramOutputModule> getHistogramModule() { return histMod; };

private:
	std::vector<double> inputDeltaRValues;
	std::vector<double> deltaPtValues;
	std::vector<double> leadingPtValues;
	std::vector<double> sumPtValues;
	std::vector<double> nParticlesValues;
	std::vector<double> etaValues;
	std::vector<double> maxIsolationValues;

	std::shared_ptr<HistogramOutputModule> histMod = std::make_shared<HistogramOutputModule>();

  LeptonJet createLeptonJet(Lepton highestPtLepton) const;
  Particle findHighestPtLepton(std::vector<Lepton> particles) const;

  std::unordered_map<std::string,std::shared_ptr<MLStripHist>> particleHistograms;

  void assignVariables(LeptonJet jet);

  int count = 0;

  std::vector<LeptonJet> leptonJets;
  std::vector<double> deltaRValues;
  std::vector<double> pTValues;

  double DeltaRCut;
};

#endif
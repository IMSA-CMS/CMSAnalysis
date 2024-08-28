#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/LeptonJetCalculation.hh"
#include <vector>

std::vector<double> LeptonJetCalculation::calculateDeltaR(LeptonJet leptonJet)
{
  double deltaR = 0;

  for (Particle p : leptonJet.getParticles()) {
    for (Particle q : leptonJet.getParticles()) {
      if (p.getDeltaR(q) > deltaR) {
        deltaR = p.getDeltaR(q);
      }
    }
  }

  return {deltaR};
}

std::vector<double> LeptonJetCalculation::calculateDeltaPt(LeptonJet leptonJet)
{
  double leadingPt = 0;
  double runnerUpPt = 0;

  for (Particle p : leptonJet.getParticles()) {
    if (p.getPt() > leadingPt) {
      runnerUpPt = leadingPt;
      leadingPt = p.getPt();
    }
  }

  return {leadingPt - runnerUpPt};
}

std::vector<double> LeptonJetCalculation::calculateSumPt(LeptonJet leptonJet)
{
  double sumPt = 0;

  for (Particle p : leptonJet.getParticles()) {
    sumPt += p.getPt();
  }

  return {sumPt};
}

std::vector<double> LeptonJetCalculation::calculateLeadingPt(LeptonJet leptonJet)
{
  double leadingPt = 0;

  for (Particle p : leptonJet.getParticles()) {
    if (p.getPt() > leadingPt) {
      leadingPt = p.getPt();
    }
  }

  return {leadingPt};
}

std::vector<double> LeptonJetCalculation::calculateMaxIsolation(LeptonJet leptonJet)
{
  double tempMaxIso = 0;

  for (Particle p : leptonJet.getParticles()) {
    if (p.getInfo("Isolation") > tempMaxIso) {
      tempMaxIso = p.getInfo("Isolation");
    }
  }

  return {tempMaxIso};
}

std::vector<double> LeptonJetCalculation::calculateNParticles(LeptonJet leptonJet)
{
  return {static_cast<double>(leptonJet.getNumParticles())};
}

std::vector<double> LeptonJetCalculation::calculateEta(LeptonJet leptonJet)
{
  return {leptonJet.getEta()};
}

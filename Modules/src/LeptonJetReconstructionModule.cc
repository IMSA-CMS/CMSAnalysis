#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Histograms/interface/MLStripHist.hh"
#include <iostream>

LeptonJetReconstructionModule::LeptonJetReconstructionModule(double deltaRCut) : DeltaRCut(deltaRCut) {}

bool LeptonJetReconstructionModule::process()  // reco::deltaR(v1, v2)
{
  // std::cout << "LeptonJetReconstruction process()";
  //  std::cout << "Lepton jet selector: " << leptonSelector << '\n';

  if (particleHistograms.size() == 0) {
    // create histograms

    auto inputDeltaRHist = std::make_shared<MLStripHist>(
        "Input Delta R Values", 100, 0, 0.5, [this]() { return this->getDeltaRValues(); });
    auto deltaPtHist = std::make_shared<MLStripHist>(
        "Input Delta Pt Values", 100, 0, 1000, [this]() { return this->getDeltaPtValues(); });
    auto leadingPtHist = std::make_shared<MLStripHist>(
        "Input Leading Pt Values", 100, 0, 1000, [this]() { return this->getLeadingPtValues(); });
    auto sumPtHist =
        std::make_shared<MLStripHist>("Input Sum Pt Values", 100, 0, 1100, [this]() { return this->getSumPtValues(); });
    auto nParticlesHist = std::make_shared<MLStripHist>(
        "Input nParticles Values", 5, 0, 5, [this]() { return this->getNParticlesValues(); });
    auto etaHist =
        std::make_shared<MLStripHist>("Input Eta Values", 100, 0, 5, [this]() { return this->getEtaValues(); });
    auto maxIsolationHist = std::make_shared<MLStripHist>(
        "Input MaxIsolation Values", 50, 0, 3, [this]() { return this->getMaxIsolationValues(); });

    particleHistograms.insert({"deltaR", inputDeltaRHist});
    particleHistograms.insert({"deltaPt", deltaPtHist});
    particleHistograms.insert({"leadingPt", leadingPtHist});
    particleHistograms.insert({"sumPt", sumPtHist});
    particleHistograms.insert({"nParticles", nParticlesHist});
    particleHistograms.insert({"eta", etaHist});
    particleHistograms.insert({"maxIsolation", maxIsolationHist});

    histMod->addHistogram(inputDeltaRHist);
    histMod->addHistogram(deltaPtHist);
    histMod->addHistogram(leadingPtHist);
    histMod->addHistogram(sumPtHist);
    histMod->addHistogram(nParticlesHist);
    histMod->addHistogram(etaHist);
    histMod->addHistogram(maxIsolationHist);
  }

  const auto &recoCandidates = getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());
  leptonJets = findLeptonJets(recoCandidates);
  findDeltaRValues();
  findPtValues();

  // std::cout << "LJ:" << leptonJets.size() << "\n";
  return true;
}

const std::vector<LeptonJet> LeptonJetReconstructionModule::findLeptonJets(ParticleCollection<Lepton> recoCandidates) {
  auto recoLeptons = recoCandidates.getParticles();
  std::vector<LeptonJet> leptonJetList;
  while (recoLeptons.size() != 0) {
    Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
    if (highestPtLepton.getPt() <= 5) {
      break;
    } else if (abs(highestPtLepton.getEta()) > 3) {
      recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));
      continue;
    }
    LeptonJet jet = createLeptonJet(highestPtLepton);
    std::vector<Particle> initialLeptons = jet.getParticles();

    auto highestPtLeptonFourVector = highestPtLepton.getFourVector();
    recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));

    for (unsigned i = 0; i < recoLeptons.size(); ++i) {
      auto fourVector = recoLeptons[i].getFourVector();
      double deltaR = reco::deltaR(highestPtLeptonFourVector, fourVector);
      // std::cout << "delta r: " << deltaR << " delta r cut: " << DeltaRCut << "\n";
      // std::cout << "The value of Eta is "<< abs(recoLeptons[i].getEta()) <<std::endl;

      if (deltaR < DeltaRCut && recoLeptons[i].getPt() >= 5 && abs(recoLeptons[i].getEta()) <= 3) {
        jet.addParticle(recoLeptons[i]);
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;
      }
    }
    // std::cout << "numParticles: " << jet.getNumParticles() << "\n";
    if (jet.getNumParticles() > 1) {
      // auto inputJets = getInput()->getJets(EventInput::RecoLevel::Reco);
      // bool close = false;
      // for (auto iJet : inputJets) {
      //   if (iJet.getDeltaR(jet) < .5) {
      //     close = true;
      //     break;
      //   }
      // }
      // if (!close) {
      // std::cout << "adding jet\n";

      count++;

      assignVariables(jet);

      leptonJetList.push_back(jet);
      //}
    } else {
      inputDeltaRValues.clear();
      deltaPtValues.clear();
      leadingPtValues.clear();
      sumPtValues.clear();
      nParticlesValues.clear();
      etaValues.clear();
      maxIsolationValues.clear();
    }
  }

  return leptonJetList;
}

void LeptonJetReconstructionModule::assignVariables(LeptonJet leptonJet) {
  double sumPt = 0;
  double deltaR = 0;
  double leadingPt = 0;
  double tempMaxIso = 0;
  double runnerUpPt = 0;

  for (Particle p : leptonJet.getParticles()) {
    for (Particle q : leptonJet.getParticles()) {
      if (p.getDeltaR(q) > deltaR) {
        deltaR = p.getDeltaR(q);
      }
    }

    double pt = p.getPt();
    if (pt > leadingPt) {
        runnerUpPt = leadingPt;  // Previous leadingPt is now runner-up
        leadingPt = pt;  // Update leadingPt to the new higher value
    } else if (pt > runnerUpPt) {
        runnerUpPt = pt;  // Update runnerUpPt if current pt is less than leadingPt but greater than runnerUpPt
    }

    sumPt += p.getPt();

    if (p.getInfo("Isolation") > tempMaxIso) {
      tempMaxIso = p.getInfo("Isolation");
    }
  }

  inputDeltaRValues = {deltaR};
  deltaPtValues = {leadingPt - runnerUpPt};
  leadingPtValues = {leadingPt};
  sumPtValues = {sumPt};
  nParticlesValues = {static_cast<double>(leptonJet.getNumParticles())};
  etaValues = {leptonJet.getEta()};
  maxIsolationValues = {tempMaxIso};
}

LeptonJet LeptonJetReconstructionModule::createLeptonJet(Lepton highestPtLepton) const {
  LeptonJet leptonJet;
  leptonJet.addParticle(highestPtLepton);
  return leptonJet;
}

Particle LeptonJetReconstructionModule::findHighestPtLepton(std::vector<Lepton> leptons) const {
  double highestPt = 0;
  for (auto lepton : leptons) {
    double pt = lepton.getPt();
    if (pt > highestPt) {
      highestPt = pt;
    }
  }

  for (auto lep : leptons) {
    if (lep.getPt() == highestPt) {
      return lep;
    }
  }

  throw std::runtime_error("No highest pT lepton!");
}

void LeptonJetReconstructionModule::findDeltaRValues() {
  deltaRValues.clear();
  for (LeptonJet jet : leptonJets) {
    auto jetParticles = jet.getParticles();

    for (Particle particle : jetParticles) {
      auto initFourVector = particle.getFourVector();
      for (Particle part : jetParticles) {
        if (part != particle) {
          auto nextFourVector = part.getFourVector();
          double deltaR = reco::deltaR(initFourVector, nextFourVector);
          //deltaRValues.push_back(deltaR);
        }
      }
    }
  }
}

void LeptonJetReconstructionModule::findPtValues() {
  pTValues.clear();
  for (LeptonJet jet : leptonJets) {
    auto jetParticles = jet.getParticles();

    for (Particle part : jetParticles) {
      double pT = part.getPt();
      pTValues.push_back(pT);
    }
  }
}
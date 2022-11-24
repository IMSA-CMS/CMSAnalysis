#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include <iostream>

LeptonJetReconstructionModule::LeptonJetReconstructionModule(double deltaRCut) :
  DeltaRCut(deltaRCut)
{
}

bool LeptonJetReconstructionModule::process() // reco::deltaR(v1, v2)
{
  leptonJets.clear();
  //std::cout << "LeptonJetReconstruction process()";
  // std::cout << "Lepton jet selector: " << leptonSelector << '\n';
  const auto & recoCandidates = getInput()->getLeptons(InputModule::RecoLevel::Reco);
  auto recoLeptons = recoCandidates.getParticles();

  while (recoLeptons.size() != 0) 
  {
    Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
    if(highestPtLepton.getPt()<= 5)
    {
      break;
    }
    else if(abs(highestPtLepton.getEta())>3)
    {
      recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));
      continue;
    }
    LeptonJet jet = createLeptonJet(highestPtLepton);
    std::vector<Particle> initialLeptons = jet.getParticles();

    auto highestPtLeptonFourVector = highestPtLepton.getFourVector();
    recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));

    for (unsigned i = 0; i < recoLeptons.size(); ++i)
    {
      auto fourVector = recoLeptons[i].getFourVector();
      double deltaR = reco::deltaR(highestPtLeptonFourVector, fourVector);
      //std::cout << "delta r: " << deltaR << " delta r cut: " << DeltaRCut << "\n";
      //std::cout << "The value of Eta is "<< abs(recoLeptons[i].getEta()) <<std::endl;
      if (deltaR < DeltaRCut && recoLeptons[i].getPt()>= 5 && abs(recoLeptons[i].getEta())<=3)
      {
        jet.addParticle(recoLeptons[i]);
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;
      }
      else if(deltaR < DeltaRCut)
      {
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;
      }
    }
  //std::cout << "numParticles: " << jet.getNumParticles() << "\n";
    if (jet.getNumParticles() > 1)
    {
      // auto inputJets = getInput()->getJets(InputModule::RecoLevel::Reco);
      // bool close = false;
      // for (auto iJet : inputJets) {
      //   if (iJet.getDeltaR(jet) < .5) {
      //     close = true;
      //     break;
      //   }
      // }
      //if (!close) {
        //std::cout << "adding jet\n";
        leptonJets.push_back(jet);
      //}
    }
  }
  findDeltaRValues();
  findPtValues();
  // std::cout << "LJ:" << leptonJets.size() << "\n";
  return true;
}

LeptonJet LeptonJetReconstructionModule::createLeptonJet(Lepton highestPtLepton) const
{
  LeptonJet leptonJet;
  leptonJet.addParticle(highestPtLepton);
  return leptonJet;
}

Particle LeptonJetReconstructionModule::findHighestPtLepton(std::vector<Lepton> leptons) const
{
  double highestPt = 0;
  for (auto lepton : leptons)
  {
    double pt = lepton.getPt();
    if (pt > highestPt)
    {
      highestPt = pt;
    }
  }

  for (auto lep : leptons)
  {
    if (lep.getPt() == highestPt)
    {
       return lep;
    }
  }

  throw std::runtime_error("No highest pT lepton!");
}

void LeptonJetReconstructionModule::findDeltaRValues()
{
  deltaRValues.clear();
  for (LeptonJet jet : leptonJets)
  {
    auto jetParticles = jet.getParticles();

    for (Particle particle : jetParticles)
    {
      auto initFourVector = particle.getFourVector();
      for (Particle part : jetParticles)
      {
        if (part != particle)
        {
          auto nextFourVector = part.getFourVector();
          double deltaR = reco::deltaR(initFourVector, nextFourVector);
          deltaRValues.push_back(deltaR);
        }
      }
    }
  }
}

void LeptonJetReconstructionModule::findPtValues()
{
  pTValues.clear();
  for (LeptonJet jet : leptonJets)
  {
    auto jetParticles = jet.getParticles();

    for (Particle part : jetParticles)
    {
      double pT = part.getPt();
      pTValues.push_back(pT);
    }
  }
}

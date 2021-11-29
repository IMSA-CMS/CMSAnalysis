#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

LeptonJetReconstructionModule::LeptonJetReconstructionModule(double deltaRCut, double ipTCut) :
  DeltaRCut(deltaRCut),
  pTCut(ipTCut)
  //reco(recoModule),
{
}

bool LeptonJetReconstructionModule::process() // reco::deltaR(v1, v2)
{
  leptonJets.clear();
  const ParticleCollection& recoCandidates = getInput()->getParticles(InputModule::RecoLevel::Reco);
  std::vector<Particle> recoLeptons = recoCandidates.getParticles();

  while (recoLeptons.size() != 0)
  {
    Particle highestPtLepton = findHighestPtLepton(recoLeptons);
    LeptonJet jet = createLeptonJet(highestPtLepton);
    std::vector<Particle> initialLeptons = jet.getParticles();

    auto highestPtLeptonFourVector = highestPtLepton.getFourVector();
    recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));

    for (unsigned i = 0; i < recoLeptons.size(); ++i)
    {
      auto fourVector = recoLeptons[i].getFourVector();
      double deltaR = reco::deltaR(highestPtLeptonFourVector, fourVector);
      if (deltaR < DeltaRCut)
      {
        jet.addParticle(recoLeptons[i]);
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;           
      }
    }

    if (jet.getNumParticles() > 1)
    {
      //if (jet.getMass() < 50)
      {
        leptonJets.push_back(jet);
      }
    }
  }
  findDeltaRValues();
  findPtValues();
  return true;
}

LeptonJet LeptonJetReconstructionModule::createLeptonJet(Particle highestPtLepton) const
{
  LeptonJet leptonJet(highestPtLepton);
  return leptonJet;
}

Particle LeptonJetReconstructionModule::findHighestPtLepton(std::vector<Particle> leptons) const
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
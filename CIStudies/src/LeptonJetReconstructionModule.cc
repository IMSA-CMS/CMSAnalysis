#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

LeptonJetReconstructionModule::LeptonJetReconstructionModule(std::shared_ptr<RecoIdentificationModule> recoModule, double deltaRCut, double ipTCut) :
  reco(recoModule),
  DeltaRCut(deltaRCut),
  pTCut(ipTCut)
{
}

bool LeptonJetReconstructionModule::process(const edm::EventBase& event) // reco::deltaR(v1, v2)
{
  leptonJets.clear();
  const ParticleCollection& recoCandidates = reco->getRecoCandidates();
  std::vector<Particle> recoLeptons = recoCandidates.getParticles();

  while (recoLeptons.size() != 0)
  {
    Particle highestPtLepton = findHighestPtLepton(recoLeptons);
    LeptonJet jet = createLeptonJet(highestPtLepton);
    std::vector<Particle> initialLeptons = jet.getParticles();

    auto highestPtLeptonFourVector = highestPtLepton.fourVector();
    recoLeptons.erase(std::find(recoLeptons.begin(), recoLeptons.end(), highestPtLepton));

    for (unsigned i = 0; i < recoLeptons.size(); ++i)
    {
      auto fourVector = recoLeptons[i].fourVector();
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
    double pt = lepton.pt();
    if (pt > highestPt)
    {
      highestPt = pt;      
    }
  }

  for (auto lep : leptons)
  {    
    if (lep.pt() == highestPt)
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
      auto initFourVector = particle.fourVector();
      for (Particle part : jetParticles)
      {
        if (part != particle)
        {
          auto nextFourVector = part.fourVector();
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
      double pT = part.pt();
      pTValues.push_back(pT);
    }
  }
}
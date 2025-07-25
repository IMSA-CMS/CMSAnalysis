
#include "CMSAnalysis/Filters/interface/FakePhotonSelector.hh"
#include <vector>


#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"


FakePhotonSelector::FakePhotonSelector(double ideltaRCut) : deltaRCut(ideltaRCut) {}
// FakePhotonSelector class/constructor initializes object with deltaRcut threshold value
void FakePhotonSelector::selectParticles(const EventInput* input, Event& event) const
{
  if (!input) return;


  // GenSim particles
  auto particlesGenSim = input->getParticles(EventInput::RecoLevel::GenSim).getParticles();
  for (const auto& particle : particlesGenSim)
  // adds generated particles to the event object
  {
    event.addGenSimParticle(particle);
  }


  // Reco particles
  ParticleCollection<Lepton> selected;
  auto particles = input->getParticles(EventInput::RecoLevel::Reco).getParticles();


  float muonCounter = 0, looseMuons = 0, tightMuons = 0, mediumMuons = 0;
  float electronCounter = 0, looseElectrons = 0, mediumElectrons = 0, tightElectrons = 0;
  float numOfParticles = 0;


  std::vector<Particle> electrons;


  for (const auto& particle : particles)
  {
    numOfParticles++;


    if (particle.getType() == ParticleType::muon() && particle.getPt() > 5)
    {
      muonCounter++;
      Lepton lepton(particle);
      if (lepton.isLoose()) looseMuons++;
      if (lepton.isTight()) tightMuons++;
      if (lepton.isMedium()) mediumMuons++;
    }


    if (particle.getType() == ParticleType::electron() && particle.getPt() > 5)
    {
      electronCounter++;
      Lepton lepton(particle);
      electrons.push_back(particle);
      if (lepton.isLoose())
      {
        selected.addParticle(particle);
        looseElectrons++;
      }
      if (lepton.isTight()) tightElectrons++;
      if (lepton.isMedium()) mediumElectrons++;
    }
  }


  // Photon selection with deltaR matching
  for (const auto& particle : particles)
  {
    if (particle.getType() == ParticleType::photon() && particle.getPt() > 5)  
    {
      double photonPt = particle.getPt();
      bool isFakePhoton = false;


      for (const auto& electron : electrons)
      {
        double electronPt = electron.getPt();
        if (std::abs(photonPt - electronPt) <= 45.0 &&
            reco::deltaR(particle.getFourVector(), electron.getFourVector()) < deltaRCut)
        {
          isFakePhoton = true;
          break;
        }
      }


      if (isFakePhoton)
      {
        event.addSpecialObject("FakePhoton", particle);
      }
    }
  }


  for (const auto& lepton : selected.getParticles())
  {
    event.addElectron(lepton);
  }


  auto leptonJets = findLeptonJets(selected);
  for (const auto& jet : leptonJets)
  {
    event.addSpecialObject("leptonJet", jet);
  }
}


std::vector<LeptonJet> FakePhotonSelector::findLeptonJets(ParticleCollection<Lepton> recoCandidates) const
{
  auto recoLeptons = recoCandidates.getParticles();
  std::vector<LeptonJet> leptonJetList;


  while (!recoLeptons.empty())
  {
    Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
    if (highestPtLepton.getPt() <= 5 || std::abs(highestPtLepton.getEta()) > 3)
    {
      recoLeptons.erase(std::remove(recoLeptons.begin(), recoLeptons.end(), highestPtLepton), recoLeptons.end());
      continue;
    }


    LeptonJet jet = createLeptonJet(highestPtLepton);
    recoLeptons.erase(std::remove(recoLeptons.begin(), recoLeptons.end(), highestPtLepton), recoLeptons.end());


    for (size_t i = 0; i < recoLeptons.size(); ++i)
    {
      if (reco::deltaR(highestPtLepton.getFourVector(), recoLeptons[i].getFourVector()) < deltaRCut)
      {
        jet.addParticle(recoLeptons[i]);
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;
      }
    }


    if (jet.getNumParticles() > 1)
    {
      leptonJetList.push_back(jet);
    }
  }
  return leptonJetList;
}


LeptonJet FakePhotonSelector::createLeptonJet(Lepton highestPtLepton) const
{
  LeptonJet leptonJet;
  leptonJet.addParticle(highestPtLepton);
  return leptonJet;
}


Particle FakePhotonSelector::findHighestPtLepton(std::vector<Lepton> leptons) const
{
  if (leptons.empty()) throw std::runtime_error("No leptons available");


  return *std::max_element(leptons.begin(), leptons.end(),
    [](const Lepton& a, const Lepton& b) { return a.getPt() < b.getPt(); });
}

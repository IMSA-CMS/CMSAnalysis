#include "CMSAnalysis/Filters/interface/LeptonJetAntiSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "DataFormats/Math/interface/deltaR.h"

LeptonJetAntiSelector::LeptonJetAntiSelector(double idXYCut, double idZCut) : dXYCut(idXYCut), dZCut(idZCut)
{ }

void LeptonJetAntiSelector::selectParticles(const EventInput* input, Event& event) const
{
  // GenSim stuff
  auto particlesGenSim = input->getParticles(EventInput::RecoLevel::GenSim).getParticles();
  for (auto particle : particlesGenSim)
  {
    event.addGenSimParticle(particle);
  }

  // Reco stuff
    ParticleCollection<Muon> selected;
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();
    
    for (const auto& particle : particles)
    {
      if (particle.getType() == ParticleType::muon() && particle.getPt() > 5) 
      {
        auto lepton = Lepton(particle);
        if(lepton.isLoose() && lepton.getDXY() > dXYCut && lepton.getDZ() > dZCut)
        {
          selected.addParticle(particle);
        }
      }
    }

    for (auto lepton : selected.getParticles())
    {
      event.addMuon(lepton);
    }

    //auto recoLeptons = event.getMuons();
    auto leptonJets = findLeptonJets(selected, 0.5);

    for (const auto& jet : leptonJets)
    {
      event.addSpecialObject("leptonJet", jet);
      bool mixed = false;
      auto type = jet.getParticles().at(0).getType();
      for (auto p : jet.getParticles())
      {
        if (p.getType() != type)
        {
          mixed = true;
          break;
        }
      }

      if (mixed)
      {
        event.addSpecialObject("mixedLeptonJet", jet);
      }
    }
}

std::vector<LeptonJet> LeptonJetAntiSelector::findLeptonJets(ParticleCollection<Lepton> recoCandidates, double DeltaRCut)
{
  auto recoLeptons = recoCandidates.getParticles();
  //std::cout << "# of muons: " << recoLeptons.size() << std::endl;
  std::vector<LeptonJet> leptonJetList;
  while (recoLeptons.size() != 0)
  {
    Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
    //std::cout << "Highest lepton pT: " << highestPtLepton.getPt() << std::endl;
    if (highestPtLepton.getPt() <= 5)
    {
      break;
    }
    else if (abs(highestPtLepton.getEta()) > 3)
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
      // std::cout << "The value of Eta is "<< abs(recoLeptons[i].getEta()) <<std::endl;
      if (deltaR < DeltaRCut && recoLeptons[i].getPt() >= 5 && abs(recoLeptons[i].getEta()) <= 3)
      {
        jet.addParticle(recoLeptons[i]);
        recoLeptons.erase(recoLeptons.begin() + i);
        --i;
      }
    }

    // std::cout << "numParticles: " << jet.getNumParticles() << "\n";
    if (jet.getNumParticles() > 1)
    {
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
      leptonJetList.push_back(jet);
      //std::cout << "Lepton jet added!" << std::endl;
      //}
    }
  }
  return leptonJetList;
}

Particle LeptonJetAntiSelector::findHighestPtLepton(std::vector<Lepton> leptons)
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

LeptonJet LeptonJetAntiSelector::createLeptonJet(Lepton highestPtLepton)
{
  LeptonJet leptonJet;
  leptonJet.addParticle(highestPtLepton);
  return leptonJet;
}
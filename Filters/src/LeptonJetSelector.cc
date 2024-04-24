#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

LeptonJetSelector::LeptonJetSelector(double ideltaRCut) : deltaRCut(ideltaRCut)
{ }

void LeptonJetSelector::selectParticles(const EventInput* input, Event& event) const
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
        if(lepton.isLoose())
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
    auto leptonJets = findLeptonJets(selected);

    for (const auto& jet : leptonJets)
    {
      event.addSpecialObject("leptonJet", jet);
    }
}

std::vector<LeptonJet> LeptonJetSelector::findLeptonJets(ParticleCollection<Lepton> recoCandidates) const
{
  auto recoLeptons = recoCandidates.getParticles();
  std::vector<LeptonJet> leptonJetList;
  while (recoLeptons.size() != 0)
  {
    Lepton highestPtLepton = findHighestPtLepton(recoLeptons);
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
      // std::cout << "delta r: " << deltaR << " delta r cut: " << DeltaRCut << "\n";
      // std::cout << "The value of Eta is "<< abs(recoLeptons[i].getEta()) <<std::endl;
      if (deltaR < deltaRCut)
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
      //}
    }
  }
  return leptonJetList;
}

LeptonJet LeptonJetSelector::createLeptonJet(Lepton highestPtLepton) const
{
  LeptonJet leptonJet;
  leptonJet.addParticle(highestPtLepton);
  return leptonJet;
}
//runAnalyzer input=darkPhotonBaselineRun2.txt analysis=LeptonJetReconstruction output=leptonjetinvmasshist.root numFiles=1
//root leptonjetinvmasshist.root then type TBrowser h
Particle LeptonJetSelector::findHighestPtLepton(std::vector<Lepton> leptons) const
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

#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

float muonCounter = 0.00;
float looseMuons = 0.00;
float tightMuons = 0.00;
float mediumMuons = 0.00;

float electronCounter = 0.00;
float looseElectrons = 0.00;
float mediumElectrons = 0.00;
float tightElectrons = 0.00;

float numOfParticles = 0.00;

LeptonJetSelector::LeptonJetSelector(double ideltaRCut) : deltaRCut(ideltaRCut)
{
}

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
    //auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();
    auto particles = input->getParticles(EventInput::RecoLevel::Reco).getParticles();

    for (const auto& particle : particles)
    {
      numOfParticles++;
      if (particle.getType() == ParticleType::muon() && particle.getPt() > 5) 
      {
        muonCounter++;
        auto lepton = Lepton(particle);
        if(lepton.isLoose())
        {
          //selected.addParticle(particle);
          looseMuons++;
        }
        if(lepton.isTight())
        {
          tightMuons++;
        }
        if(lepton.isMedium())
        {
          mediumMuons++;
        }
      }

      if (particle.getType() == ParticleType::electron() && particle.getPt() > 5) 
      {
        electronCounter++;
        auto lepton = Lepton(particle);
        if(lepton.isLoose())
        {
          selected.addParticle(particle);
          looseElectrons++;
        }
        if(lepton.isTight())
        {
          tightElectrons++;
        }
        if(lepton.isMedium())
        {
          mediumElectrons++;
        }    
      }

      // Trying adding photons as well
      if (particle.getType() == ParticleType::photon() && particle.getPt() > 5)  // put work in this if statement 
      {
        // for (const Particle& particle: particles) 
        // // outer loop - particle is part of the particles container
        // {
        //   if (particle.getType() == ParticleType::photon()) 
        //   // if the particle type is photon, it's photonPt is stored
        //   {
            double photonPt = particle.getPt();
            bool withinElectronPtRange = false; 
            // boolean flag tracks when Pt is within the acceptable range
              for (const Particle& otherParticle : particles)
              {
                if (otherParticle.getType() == ParticleType::electron())
                {
                  double electronPt = otherParticle.getPt();

                  if (std::abs(photonPt - electronPt) <= 45.0)
                  {
                    withinElectronPtRange = true;
                  }

                  if (withinElectronPtRange)
                  {
                    selected.addParticle(particle);
                  }
                }
              }
         // }
       // }
    }

    for (auto lepton : selected.getParticles())
    {
      //event.addMuon(lepton);
      event.addElectron(lepton);
    }

    //auto recoLeptons = event.getMuons();
    auto leptonJets = findLeptonJets(selected);

    for (const auto& jet : leptonJets)
    {
      event.addSpecialObject("leptonJet", jet);
    }
      // std::cout<<"number of particles: " << numOfParticles << "\n";

      // std::cout<<"muon efficiency: " << (muonCounter/numOfParticles)<<"\n";
      // std::cout<<"loose muon efficiency: " << (looseMuons/numOfParticles)<<"\n";
      // std::cout<<"Medium Muon efficiency: " << mediumMuons/numOfParticles << "\n";
      // std::cout<<"Tight Muon efficiency: " << tightMuons/numOfParticles << "\n\n";

      // std::cout<<"Electron efficiency: " << (electronCounter/numOfParticles)<<"\n";
      // std::cout<<"loose electron efficiency: " << (looseElectrons/numOfParticles)<<"\n";
      // std::cout<<"Medium electron efficiency: " << mediumElectrons/numOfParticles << "\n";
      // std::cout<<"Tight electron efficiency: " << tightElectrons/numOfParticles << "\n\n";
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
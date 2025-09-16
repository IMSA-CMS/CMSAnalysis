#include "CMSAnalysis/Filters/interface/LostElectronSelector.hh"
#include <vector>
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "DataFormats/Math/interface/deltaR.h"




LostElectronSelector::LostElectronSelector(double ideltaRCut) : deltaRCut(ideltaRCut) {}
// LostElectronSelector class/constructor initializes object with deltaRcut threshold value
void LostElectronSelector::selectParticles(const EventInput* input, Event& event) const
{
  if (!input) return;

  // GenSim particles
  auto particlesGenSim = input->getParticles(EventInput::RecoLevel::GenSim).getParticles();
  for (const auto& particle : particlesGenSim)
  // adds generated particles to the event object
  {
    GenSimParticle genSimPart(particle);    
    event.addGenSimParticle(particle);
  }




  // Reco particles
  ParticleCollection<Lepton> selected;
  auto particles = input->getParticles(EventInput::RecoLevel::Reco).getParticles();


  std::vector<Particle> electrons;


  for (const auto& particle : particles)
  {


    if (particle.getType() == ParticleType::electron() && particle.getPt() > 5)
    {
      electrons.push_back(particle);
    }
  }


  std::vector<GenSimParticle> genSimElectrons;


  for (const auto& particleGenSim : particlesGenSim)
  {
    if (particleGenSim.getType() == ParticleType::electron() && particleGenSim.getPt() > 5)
    {
      GenSimParticle genSimPart(particleGenSim);
      genSimElectrons.push_back(genSimPart);
    }
  }
//look for unique mother (dark photon makes it real electron) match to recoelectrons, one match makes other lost

for (const auto& particleGenSim : particlesGenSim)
{
  GenSimParticle genSimPart(particleGenSim);
  if (particleGenSim.getType() == ParticleType::electron())
  {
    bool particleHasRecoMatch = false;
    double particleGenSimPt = particleGenSim.getPt();
    double particleGenSimEta = particleGenSim.getEta();
    double particleGenSimPhi = particleGenSim.getPhi();

      //can the same particle match with itself?
      for (const auto& genSimElectron : genSimElectrons)
      {
        bool electronHasRecoMatch = false;
        if (GenSimParticle::sharedMother(4900022, genSimPart, genSimElectron).pdgId() == 4900022)
        {
          double genSimElectronePt = genSimElectron.getPt();
          double genSimElectronEta = genSimElectron.getEta();
          double genSimElectronPhi = genSimElectron.getPhi();




          for (const auto& electron : electrons)
          {
            double electronPt = electron.getPt();
            double electronEta = electron.getEta();
            double electronPhi = electron.getPhi();
            if (std::abs(electronPt - particleGenSimPt) <= 45 && std::abs(electronEta - particleGenSimEta) <= 10 && std::abs(electronPhi - particleGenSimPhi) <= 10)
            {
              particleHasRecoMatch = true;
            }


            else if (std::abs(electronPt - genSimElectronePt) <= 45 && std::abs(electronEta - genSimElectronEta) <= 10 && std::abs(electronPhi - genSimElectronPhi) <= 10)
            {
              electronHasRecoMatch = true;
            }




            if (electronHasRecoMatch == true && particleHasRecoMatch == true)
            {
              break;
            }
          }
        }




        if (electronHasRecoMatch == false && particleHasRecoMatch == true)
        {
          event.addSpecialObject("LostElectron", genSimElectron);
          break;
        }




        else if (electronHasRecoMatch == true && particleHasRecoMatch == false)
        {
          event.addSpecialObject("LostElectron", particleGenSim);
          break;
        }
      }
    }

  }
}

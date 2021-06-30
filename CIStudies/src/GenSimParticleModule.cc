#include "CIAnalysis/CIStudies/interface/GenSimParticleModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/FWLite/interface/Event.h"

GenSimParticleModule::GenSimParticleModule(int itargetPdgId):
  targetPdgId(itargetPdgId)
{}

bool GenSimParticleModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING GenSimParticleModule" << std::endl;
  genParticles.clear();

  //Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
 
  //Begin GEN looping
  //Loop through Particle list&
  for (const auto& p : *genParticlesHandle)
  {
    if (p.pdgId() == targetPdgId)
    {
      int numDaughters = p.numberOfDaughters();
      if (numDaughters == 0)
      {
        genParticles.addParticle(Particle(&p, Particle::LeptonType::None));
      }
      else
      {
        int counter = 0;
        for (int i = 0; i < numDaughters; ++i)
        {
          auto partDaughter = p.daughter(i);
          if (p.pdgId() == partDaughter->pdgId())
          {
            counter += 1;
          }
        }
        if (counter == 0)
        {
          genParticles.addParticle(Particle(&p, Particle::LeptonType::None));
        }
      }
    }
    //std::cout << "Number of particles: " << genParticles.getNumParticles() << std::endl;
  }
  //std::cerr << "EXITING GenSimParticleModule" << std::endl;
 return true;
}
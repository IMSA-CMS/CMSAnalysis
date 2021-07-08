

#include "CIAnalysis/CIStudies/interface/GenSimStudyModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
  




GenSimStudyModule::GenSimStudyModule()
{
  
}
bool GenSimStudyModule::process(const edm::EventBase &event) {
  isPhoton = 0;
  // Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);

  
  // Begin GEN looping
  // Loop through Particle list
  for (const auto &p : *genParticlesHandle) {
    if (p.pdgId() ==  22 && p.isPromptFinalState()) {
      isPhoton++;
    }
  }
  std::cout<<"The number of photons are: " << isPhoton << '\n';
  return true;
}


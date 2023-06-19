#include "CMSAnalysis/Modules/interface/GenSimStudyModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
  




GenSimStudyModule::GenSimStudyModule()
{
  
}
bool GenSimStudyModule::process() {
  isPhoton = 0;
  // Get Events Tree and create handle for GEN

  //edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  //event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);
  auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim).getParticles();

  
  // Begin GEN looping
  // Loop through Particle list
  for (const auto &p : genParticles) {
    auto particle  = GenSimParticle(p);
    if (particle.pdgId() ==  22 && particle.isFinalState()) {
      isPhoton++;
    }
  }
  std::cout<<"The number of photons are: " << isPhoton << '\n';
  return true;
}


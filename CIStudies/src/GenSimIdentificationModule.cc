#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

GenSimIdentificationModule::GenSimIdentificationModule(int itargetPdgId)
    : targetPdgId(itargetPdgId) {}

bool GenSimIdentificationModule::process(const edm::EventBase &event) {
  // std::cerr << "ENTERING GenSimIdentificationModule" << std::endl;
  genParticles.clear();

  // Get Events Tree and create handle for GEN

  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);

  const int electronCode = 11;
  const int muonCode = 13;

  auto particle = getFileParams().getLeptonType();

  // std::cerr << "particle = " << particle << std::endl;

  // Begin GEN looping
  // Loop through Particle list&
  for (const auto &p : *genParticlesHandle) {
    if (isParticle(Particle(&p, Particle::LeptonType::None))) {
      // std::cerr << "genSim particle type = " << std::abs(p.pdgId())
      //           << std::endl;
      // std::cerr << "electronCode = " << electronCode << std::endl;
      // std::cerr << "muonCode = " << muonCode << std::endl;
      switch (std::abs(p.pdgId())) {
      case electronCode:
        if (particle == "Electron" || particle == "Both") {
          genParticles.addParticle(
              Particle(&p, Particle::LeptonType::Electron));
        }
        break;
      case muonCode:
        if (particle == "Muon" || particle == "Both") {
          genParticles.addParticle(Particle(&p, Particle::LeptonType::Muon));
        }
        break;
      }
    }
  }
  // std::cout << "LENGTH: " << genParticles.getNumParticles() << "\n";
  return true;
}

bool GenSimIdentificationModule::isParticle(Particle p) const {
  /*
    std::cout << "Printing GenSim Particle: pdgID: " << p.pdgId()
              << ", status:" << p.status() << std::endl;
  */

  /*
     PDG ID:
     https://twiki.cern.ch/twiki/bin/view/Main/PdgId
     1-6: Quarks
     7,8: b', t' quarks
     11-13: electron, electron neutrino, muon
     14-18: more massive lepton (not practical)
     21-24: gluon, photon, z0, w+
     25, 32-37: less comon boson
  */

  const int finalStateParticleStatusCode = 1;
  const int maxQuarkCode = 6;
  const int maxLeptonCode = 13;
  const int maxBosonCode = 24;

  /*
    if (p.status() != finalStateParticleStatusCode ||
        p.mother().status() == finalStateParticleStatusCode)
      return false;
  */

  if (targetPdgId != 0) {
    if (p.isNotNull() && p.mother().isNotNull()) {
      auto mother = p.mother();
      if (std::abs(mother.pdgId()) == targetPdgId) {
        // loop over mother's daughters (i.e. check siblings)
        for (int i = 0; i < mother.numberOfDaughters(); ++i) {
          // if any are the same as the targetPdgId,
          // then particle was emitted instead of
          // being a direct product of decay
          if (std::abs(mother.daughter(i).pdgId()) == targetPdgId) {
            return false;
          }
        }
        // if no siblings are the same as the parent,
        // it is a true decay
        return true;
      }
    }
    // particle/parent non existent or parent not matching pdgId
    return false;
    /*
        Particle currentMother = p;
        while (currentMother.mother().isNotNull()) {
          if (std::abs(currentMother.mother().pdgId()) == targetPdgId) {
            return true;
          }
          currentMother = currentMother.mother();
        }
        return false;
    */
  } else {
    Particle nu = p.mother();
    int motherId = nu.pdgId();
    bool isParticle = true;

    // std::cout << "Printing GenSim Mother P: pdgID: " << nu.pdgId() << ",
    // status: " << nu.status() << std::endl;

    while (std::abs(motherId) > maxQuarkCode) // not a quark
    {
      if (nu.mother().isNotNull() && nu.isNotNull()) {
        nu = nu.mother();
        motherId = nu.pdgId();

        if (std::abs(motherId) > maxLeptonCode &&
            std::abs(motherId) <= maxBosonCode) { // not a particle
          // isParticle = false;
          return false;
        }
        if (std::abs(motherId) <= maxQuarkCode) { // is a quark
          isParticle = true;
        }
      } else {
        // This is pretty weird behavior, but clearly not a good particle if it
        // gets here
        return false;
      }
    }
    return isParticle;
  }
}

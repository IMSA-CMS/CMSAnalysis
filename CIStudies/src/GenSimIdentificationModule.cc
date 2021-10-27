#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

#include <cmath>
#include <stdexcept>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

GenSimIdentificationModule::GenSimIdentificationModule(int itargetPdgId,
                                                       bool iignoreRadiation)
    : targetPdgId(itargetPdgId), ignoreRadiation(iignoreRadiation) {}

bool GenSimIdentificationModule::process(const edm::EventBase &event) {
  // std::cerr << "ENTERING GenSimIdentificationModule" << std::endl;
  genParticles.clear();

  // Get Events Tree and create handle for GEN

  
  event.getByLabel(std::string("prunedGenParticles"), genParticlesHandle);

  // const int electronCode = 11;
  // const int muonCode = 13;

  auto particle = getFileParams().getLeptonType();

  //int targetCode = particle == "Electron" ? electronCode : muonCode;

  // std::cerr << "particle = " << particle << std::endl;

  // Begin GEN looping
  // Loop through Particle list&
  for (const auto &p : *genParticlesHandle) {
    // std::cout << "One\n";
    if (p.status() != 0 && isParticle(Particle(&p))) {
      // std::cout << "Two\n";
      // std::cerr << "genSim particle type = " << std::abs(p.pdgId())
      //           << std::endl;
      // std::cerr << "electronCode = " << electronCode << std::endl;
      // std::cerr << "muonCode = " << muonCode << std::endl;

      genParticles.addParticle(Particle(&p));
      // std::cout << "Three\n";
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
  // std::cout << "Four\n";
  const int finalStateParticleStatusCode = 1;
  const int maxQuarkCode = 6;
  const int maxLeptonCode = 13;
  const int maxBosonCode = 24;

  if (p.status() != finalStateParticleStatusCode ||
      p.mother().status() == finalStateParticleStatusCode)
    return false;

  if (targetPdgId != 0) {
    // std::cout << "Five\n";
    Particle currentMother = p;
    while (currentMother.mother().isNotNull()) {
      Particle newMother = currentMother.mother();
      // std::cout << newMother.isNotNull()<<"\n";
      if (std::abs(newMother.pdgId()) == targetPdgId) {
        // std::cout << "pdgId One\n";
        if (ignoreRadiation) {
          for (int i = 0; i < newMother.numberOfDaughters(); ++i) {
            if (std::abs(newMother.daughter(i).pdgId()) == targetPdgId) {
              // std::cout << "pdgId One\n";
              return false;
            }
          }
        }
        return true;
      }
      currentMother = newMother;
    }
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

    // std::cout << "Printing GenSim Mother P: pdgID: " << std::endl;

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

ParticleCollection GenSimIdentificationModule::getPhotons() const{
  //isPhoton = 0;
  // Get Events Tree and create handle for GEN
  ParticleCollection photonList;
  // Begin GEN looping
  // Loop through Particle list
  for (const auto &p : *genParticlesHandle) {
    if (p.pdgId() ==  22 && p.isPromptFinalState()) {
      if (p.status() != 0 && isParticle(Particle(&p))) {
        photonList.addParticle(Particle(&p));
    }
    }
  }
  //std::cout<<"The number of photons are: " << isPhoton << '\n';
  return photonList;
}

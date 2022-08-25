#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/CandidateImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/SimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/DelphesImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"

Particle::Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, double relIso, Particle::SelectionFit fit):
particle(std::make_shared<SimpleImplementation>(vec, charge, type, relIso, fit))
{

}

Particle::Particle(const reco::Candidate* iparticle):
particle(std::make_shared<CandidateImplementation>(iparticle))
{

}

Particle::Particle(const LeptonJet& leptonjet):
  particle(std::make_shared<LeptonJetImplementation>(
        std::make_shared<LeptonJet>(leptonjet)))
{
}

Particle::Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, int pid, int status, int m1, int m2,int d1, int d2, double relIso):
particle(std::make_shared<DelphesImplementation>(vec,charge,type,pid,status,m1,m2,d1,d2,relIso))
{
}


Particle::Particle(const Particle& particle1):
particle(particle1.particle)
{

}

Particle& Particle::operator = (const Particle& particle2)
{
  particle = particle2.particle;
  return *this;
}

bool Particle::operator == (const Particle& p1) const
{
  return *particle == *(p1.particle);
}

bool Particle::isNotNull() const
{
  return particle->isNotNull();
}

void Particle::checkIsNull() const
{
  //std::cout << "This is check is null (particle)\n" << particle << "\n" << typeid(*particle).name() << "\n";
  if(!particle->isNotNull())
  {
    throw std::runtime_error("attempted to use null pointer in Particle (Particle)");
  }
}

double Particle::getPt() const
{
  checkIsNull();
  return particle->getFourVector().Pt();
}

double Particle::getPhi() const
{
  checkIsNull();
  return particle->getFourVector().Phi();
}

double Particle::getEta() const
{
  checkIsNull();
  return particle->getFourVector().Eta();
}
double Particle::getEt() const
{
  checkIsNull();
  return particle->getFourVector().Et();
}

double Particle::getEnergy() const
{
  checkIsNull();
  return particle->getFourVector().E();
}
double Particle::getMass() const
{
  checkIsNull();
  return particle->getFourVector().mass();
}

// double Particle::getIsolation() const
// {
//   checkIsNull();
//   return particle->isolation();
// }

Particle::BarrelState Particle::getBarrelState() const
{
  checkIsNull();
  double etaValue = std::abs(getEta());
  if (getType() == Particle::Type::Muon)
  {
    if (etaValue < 1.2)
    {
      return Particle::BarrelState::Barrel;
    }
    else
    {
      return Particle::BarrelState::Endcap;
    }
  }
  else if (getType() == Particle::Type::Electron)
  {
    if (etaValue < 1.4442)
    {
      return Particle::BarrelState::Barrel;
    }
    else if (etaValue > 1.562)
    {
      return Particle::BarrelState::Endcap;
    }
  }
  return Particle::BarrelState::None;
}

/*
double Particle::hadEt() const {
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  if (!muon) {
    return -1;
  }
  return muon->isolationR03().hadEt;
}

double Particle::hoEt() const {
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  return muon->isolationR03().hoEt;
}

double Particle::hadVetoEt() const {
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  return (!muon) ? -1 : muon->isolationR05().sumPt;
}

double Particle::trackerVetoPt() const {
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  return muon->isolationR03().trackerVetoPt;
}

bool Particle::isIsolated() const
{
  checkIsNull();
  auto muon = dynamic_cast<const reco::Muon*>(particle);
  if (!muon)
  {
    return false;
  }
  auto isolation = muon->isolationR03();
  if (isolation.sumPt < 0.1*muon->pt())
  {
    return true;
  }
  return false;
*/
Particle::Type Particle::getType() const{
    checkIsNull();
    return particle->getType();
}


// bool Particle::isIsolated() const
// {
//   checkIsNull();
//   auto muon = dynamic_cast<const reco::Muon*>(particle);
//   if (!muon)
//   {
//     return false;
//   }
//   auto isolation = muon->isolationR03();
//   if (isolation.sumPt < 0.1*muon->pt())
//   {
//     return true;
//   }
//   return false;
// }

Particle::Type Particle::identifyType(int pdgid)
{
    if (pdgid == 11 || pdgid == -11)
    {
      return Particle::Type::Electron;
    }

    else if (pdgid == 13 || pdgid == -13)
    {
      return Particle::Type::Muon;
    }
    else if (pdgid == 22)
    {
      return Particle::Type::Photon;
    }
    else if (pdgid == 4900022)
    {
      //std::cout << "Dark Photon\n";
      return Particle::Type::DarkPhoton;
    }
    else if (pdgid == 1000022)
    {
      return Particle::Type::Neutralino;
    }
    else
    {
      //std::cout << "Type: None\n";
      return Particle::Type::None;
    }
}
int Particle::getCharge() const
{
  checkIsNull();
  return particle->charge();
}




reco::Candidate::LorentzVector Particle::getFourVector() const
{
  checkIsNull();
  return particle->getFourVector();
}


double Particle::getDeltaR(Particle part) const
{
  return reco::deltaR(part.getFourVector(), getFourVector());
}

std::shared_ptr<ParticleImplementation> Particle::getParticleImplementation() {
  return particle;
}

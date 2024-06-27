#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"
#include "CMSAnalysis/Utility/interface/CandidateImplementation.hh"
#include "CMSAnalysis/Utility/interface/SimpleImplementation.hh"
#include "CMSAnalysis/Utility/interface/DelphesImplementation.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/LeptonJetImplementation.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

//added for dxy, dz
Particle::Particle(reco::Candidate::LorentzVector vec, double dxy, double dz, int pid, Particle::SelectionFit fit)
{
  auto type = ParticleType::getPDGType(pid);
  int charge = type.getCharge()* (std::signbit(pid) ? -1 : 1);
  particle = std::make_shared<SimpleImplementation>(vec, dxy, dz, charge, type, fit);
}

Particle::Particle(reco::Candidate::LorentzVector vec, double dxy, double dz, int charge, const ParticleType& type, Particle::SelectionFit fit):
particle(std::make_shared<SimpleImplementation>(vec, dxy, dz, charge, type, fit))
{
}


Particle::Particle(const reco::Candidate* iparticle):
particle(std::make_shared<CandidateImplementation>(iparticle))
{
}

Particle::Particle(const LeptonJet* iparticle):
particle(std::make_shared<LeptonJetImplementation>(iparticle))
{
}

Particle::Particle(reco::Candidate::LorentzVector vec, int pid, int status, int m1, int m2,int d1, int d2)
{
  auto type = ParticleType::getPDGType(pid);
  int charge = type.getCharge() * (pid < 0 ? -1 : 1);
  particle = std::make_shared<DelphesImplementation>(vec,charge,type,pid,status,m1,m2,d1,d2);
}


Particle::Particle(reco::Candidate::LorentzVector vec, int pid, const Particle* motherParticle, std::vector<const GenSimParticle*> daughters, const int status)
{
  auto type = ParticleType::getPDGType(pid);
  int charge = type.getCharge() * (pid < 0 ? -1 : 1);
  //static int counter = 0;
  // if (pid == 11 || pid == -11)
  // {
  //   counter++;
  // }

  static int positiveCharge = 0;
  static int negativeCharge = 0;
  
  if (charge == 1)
  {
    positiveCharge++;
    //std::cout<<"The positive charge counter: " << positiveCharge <<"\n";
  }
  else if (charge == -1)
  {
    negativeCharge++;
    //std::cout<<"The negative charge counter" << negativeCharge <<"\n";
  }

  particle = std::make_shared<GenSimSimpleImplementation>(vec, charge, type, motherParticle, daughters, status);

}

Particle::Particle(const Particle& particle1):
particle(particle1.particle)
{
}

Particle::Particle(const std::shared_ptr<ParticleImplementation> particlePtr):
particle(particlePtr){
}

Particle Particle::nullParticle(){
  return Particle(std::make_shared<CandidateImplementation>(nullptr));
}

std::string Particle::getName() const
{
  return getType().getName();
}

Particle &Particle::operator=(const Particle &particle2)
{
  particle = particle2.particle;
  return *this;
}

bool Particle::operator==(const Particle &p1) const
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
  if (!particle->isNotNull())
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
double Particle::getGamma() const
{
  checkIsNull();
  return particle->getFourVector().Gamma();
}


double Particle::getInfo(std::string mapKey) const
{
  checkIsNull();
  return particle->getInfo(mapKey);
}

void Particle::addInfo(std::string mapKey, double value)
{
  checkIsNull();
  return particle->addInfo(mapKey, value);
}

// double Particle::getIsolation() const
// {
//   checkIsNull();
//   return particle->isolation();
// }

// double Particle::getDxy() const
// {
//     return particle->dxy();
// }

// double Particle::getDz() const
// {
//   return particle->dz();
// }

Particle::BarrelState Particle::getBarrelState() const
{
  checkIsNull();
  double etaValue = std::abs(getEta());
  if (getType() == ParticleType::muon())
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
  else if (getType() == ParticleType::electron())
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
const ParticleType& Particle::getType() const{
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

std::shared_ptr<ParticleImplementation> Particle::getParticleImplementation() 
{
  return particle;
}
std::ostream& operator<<(std::ostream& str, Particle part)
{
  str << "| " << std::setw(9) << part.getType().getpdgId() << "| ";
  // Particle properties
  str << std::setw(13) << part.getCharge() << "| " << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13) << part.getPhi() << "| ";

  str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();

  return str;
}

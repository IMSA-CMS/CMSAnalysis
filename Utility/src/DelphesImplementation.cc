
#include "CMSAnalysis/Utility/interface/DelphesImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

DelphesImplementation::DelphesImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type,int pid, int status, int m1, int m2,int d1, int d2, double iIsolation ):
lorentzVec(vec), 
particleCharge(ch),
particleType(type),
particleIsolation(iIsolation),
particleID(pid),
statusCode(status)
{

  mothersVec.emplace_back(m1);
  mothersVec.emplace_back(m2);
  daughtersVec.emplace_back(d1);
  daughtersVec.emplace_back(d1);
}

bool DelphesImplementation::operator== (const ParticleImplementation& other) const 
{
    try
    {
      auto otherImp = dynamic_cast<const DelphesImplementation&>(other);
      return otherImp.lorentzVec == lorentzVec && otherImp.particleCharge == particleCharge;
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}

reco::Candidate::LorentzVector DelphesImplementation::getFourVector() const
{
  // HELP WHY ARE THE MASSES NEGATIVE [ATANG, 11/07]
  // std::cout << lorentzVec.mass() << std::endl;
  return lorentzVec;
}


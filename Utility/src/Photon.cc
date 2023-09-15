#include "CMSAnalysis/Utility/interface/Photon.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"
#include "CMSAnalysis/Utility/interface/CandidateImplementation.hh"
#include "CMSAnalysis/Utility/interface/SimpleImplementation.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/LeptonJetImplementation.hh"

Photon::Photon(const Particle &pParticle):
RecoParticle(pParticle)
{

}


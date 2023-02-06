#include "CMSAnalysis/DataCollection/interface/Jet.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/CandidateImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/SimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"

Jet::Jet(const Particle &pParticle):
Particle(pParticle)
{

}
// bool isBJet()
// {
// 	if (particle.getParticle().getInfo("bJet") == 1)
// 	{
// 		return true;
// 	}
// 	return false;
// }
#include "CMSAnalysis/Utility/interface/Lepton.hh"
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

Lepton::Lepton(const Particle &pParticle):
Particle(pParticle)
{

}
// double Lepton::getIsolation() const GAVIN CHANGED
// {
//   checkIsNull();
//   return getParticle()->getInfo("Isolation");
// }

bool Lepton::isTight() const
{
    checkIsNull();
    return getParticle()->getSelectionFit() == Particle::SelectionFit::Tight;
}

bool Lepton::isMedium() const
{
    checkIsNull();
    return isTight() || getParticle()->getSelectionFit() == Particle::SelectionFit::Medium;
}

bool Lepton::isLoose() const
{
    checkIsNull();
    return isMedium() || getParticle()->getSelectionFit() == Particle::SelectionFit::Loose;
}

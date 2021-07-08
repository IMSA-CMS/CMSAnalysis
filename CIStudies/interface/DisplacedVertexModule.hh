#ifndef DISPLACEDVERTEXMODULE_HH
#define DISPLACEDVERTEXMODULE_HH

#include "ProductionModule.hh"
#include "GenSimIdentificationModule.hh"
#include "RecoIdentificationModule.hh"
#include "MatchingPairCollection.hh"

#include "TLorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

// --- Not currently working --- //

class DisplacedVertexModule : public ProductionModule
{
public:
  DisplacedVertexModule(const std::shared_ptr<RecoIdentificationModule> recoModule);
  virtual bool process(const edm::EventBase& event) override;

  void propagateTrack(reco::TrackRef track);

private:
  const std::shared_ptr<RecoIdentificationModule> reco; 
};

#endif



#ifndef DISPLACEDVERTEXMODULE_HH
#define DISPLACEDVERTEXMODULE_HH

#include "ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/MatchingPairCollection.hh"
#include "DataFormats/GeometrySurface/interface/Surface.h"

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
  DisplacedVertexModule();
  virtual bool process() override;

  Surface::GlobalPoint propagateTrack(reco::TrackRef& track, float rmin);

  double closestApproach(reco::TrackRef& t1, reco::TrackRef& t2);

  virtual std::string getName() override {return "DisplacedVertexModule";}
};

#endif



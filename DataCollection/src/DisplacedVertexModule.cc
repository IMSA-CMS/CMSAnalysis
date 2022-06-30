#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "MagneticField/UniformEngine/interface/UniformMagneticField.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include <limits>

#include "CMSAnalysis/DataCollection/interface/CandidateImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"

#include <algorithm>
#include <cmath>
#include <tuple>
#include <cfloat>

#include <iostream> 

// TODO: Write distance(Muon m1, Muon m2, double radius) function
// Radius is radius of the cylinder, find the distance between the two muons as they hit the cylinder.
// Add radius parameters to the module 

// --- Not currently working --- //

/*
using Surface::GlobalPoint;
using Surface::GlobalVector;
using Cylinder::PositionType;
using Cylinder::RotationType;
//typedef Surface::Cylinder Cylinder;
*/

DisplacedVertexModule::DisplacedVertexModule()
{
}

bool DisplacedVertexModule::process()
{
  std::vector<Lepton> recoCandidates(getInput()->getLeptons(InputModule::RecoLevel::Reco).getParticles());

  for (auto& inputPart : recoCandidates)
  {
    auto part = std::dynamic_pointer_cast<CandidateImplementation>(inputPart.getParticleImplementation());
    for(auto& inputPart2 : recoCandidates)
    {
      auto part2 = std::dynamic_pointer_cast<CandidateImplementation>(inputPart2.getParticleImplementation());
      if(part2 == part)
      {
        continue;
      }
      else
      {

        // TODO: fix getUnderlyingParticle function call. The function does not exist anymore.
        // getUnderlyingParticle used to return a Candidate pointer that could be casted into a muon.

        // Particle class has particle implementation, and Candidate is below that. This is what
        // we want to find.

        auto particle = part->getUnderlyingParticle();
        auto particle2 = part2->getUnderlyingParticle();
    
        auto muon = dynamic_cast<const pat::Muon*>(particle);
        auto muon2 = dynamic_cast<const pat::Muon*>(particle2);

        if (muon && muon2)
        {
          auto muonTrack = muon->track();
          auto muonTrack2 = muon2->track();
          // auto muonTrack = muon->outerTrack();

          if (!muonTrack || !muonTrack2) 
          {
            std::cout << "NO TRACK" << std::endl;
          }
          else
          {
            try
            {
              muonTrack->innerPosition();
              std::cout << "Closest Approach: " << closestApproach(muonTrack, muonTrack2) /* <-- exceptions */ << std::endl;
            }
            catch(edm::Exception& ex)
            {
              std::cout << "Something broke" << std::endl;
              std::cout << ex.what() << std::endl;
            }
          }
        }
        else
        {
          // std::cout << "NOT A MUON" << std::endl; // Commented this line because it gets really annoying to look at.
        }
      }
    }
  }

  return true; 
}

Surface::GlobalPoint DisplacedVertexModule::propagateTrack(reco::TrackRef& track, float rmin)
{
  Surface::GlobalPoint bestPoint;
  std::cout << "Propagating Track." << std::endl;
  Surface::GlobalVector magneticFieldVector(0, 0, 4); // wrong magnetic field
  // std::cout << "Magnetic Field Vector" << std::endl;

  // Everything seems to need a regular Magnetic Field, but that's an abstract class?
  const UniformMagneticField* magneticField = new UniformMagneticField(magneticFieldVector);
  // std::cout << "Magnetic field." << std::endl;
  /*
  auto& outerPoint = track->outerPosition();
  auto& outerMomentum = track->outerMomentum();

  Surface::GlobalPoint trackPoint(outerPoint.x(), outerPoint.y(), outerPoint.z());
  Surface::GlobalVector trackMomentum(outerMomentum.x(), outerMomentum.y(), outerMomentum.z());
  */

  auto& innerPoint = track->innerPosition(); // causes errors
  std::cout << "Inner Position made." << std::endl;
  auto& innerMomentum = track->innerMomentum();
  std::cout << "Position and momentum set." << std::endl;

  Surface::GlobalPoint trackPoint(innerPoint.x(), innerPoint.y(), innerPoint.z());
  Surface::GlobalVector trackMomentum(innerMomentum.x(), innerMomentum.y(), innerMomentum.z());

  std::cout << "INNER POINT --> " << innerPoint.x() << " " << innerPoint.y() << std::endl;

  TrackCharge trackCharge = 1; // I assume this or -1? Why assume in the first place?

  FreeTrajectoryState* muonTrajectory = new FreeTrajectoryState(trackPoint, trackMomentum, trackCharge, magneticField);
  std::cout << "fts X --> " << muonTrajectory->position().x() << std::endl;
  
  AnalyticalPropagator* propagator = new AnalyticalPropagator(magneticField, oppositeToMomentum);

  
  
  const Cylinder::PositionType cylinderPos(0, 0, 0);
  const Cylinder::RotationType cylinderRot(1, 0, 0, 0, 1, 0, 0, 0, 1); // fix these, these types are confusing
  
  float rmax = rmin + 0.1;
  float zmin = -50;
  float zmax = 50;

  SimpleCylinderBounds cylinderBounds(rmin, rmax, zmin, zmax);
  Cylinder barrelCylinder(cylinderPos, cylinderRot, cylinderBounds);

  BasicSingleTrajectoryState* basicTrajectory = new BasicSingleTrajectoryState(*muonTrajectory, barrelCylinder);
  TrajectoryStateOnSurface* trajectoryOnSurface = new TrajectoryStateOnSurface(basicTrajectory);

  std::pair<TrajectoryStateOnSurface, double> propagateWithPathTSOS = propagator->propagateWithPath(*trajectoryOnSurface, barrelCylinder);

  // std::cout << "TSOS Position --> " << propagateWithPathTSOS.second << std::endl;

  // To stop the segfaults, we just ignore the path and return a default each time it isn't valid.
  if(!propagateWithPathTSOS.first.isValid()) {
    std::cout << "Invalid Path" << std::endl;
    return Surface::GlobalPoint();
  }

  std::cout << "TSOS Position --> " << /* seg faults here -> */ propagateWithPathTSOS.first.globalPosition().x() << " " << propagateWithPathTSOS.first.globalPosition().y() << std::endl;
  bestPoint = propagateWithPathTSOS.first.globalPosition();
  // std::cout << "TSOS Position --> " << propagateWithPathTSOS.first.data() << std::endl;

  return bestPoint;
}

double DisplacedVertexModule::closestApproach(reco::TrackRef& t1, reco::TrackRef& t2) {
  std::vector<Surface::GlobalPoint> p1Tracks, p2Tracks;

  double rmin = 2;

  while(rmin > 0) {
    Surface::GlobalPoint p1Add = propagateTrack(t1, rmin);
    Surface::GlobalPoint p2Add = propagateTrack(t2, rmin);

    // Since I'm not sure if there's a more efficient way to check this, to see if a
    // track needs to be removed, we just check that it isn't the same as the default
    // value of the propagateTrack method.
    // 
    // Note: != is not defined as an operator on Surface::GlobalPoint, while == is. Thus,
    // the negation is performed as a separate operation.
    if(!(p1Add == Surface::GlobalPoint()))
    {
      p1Tracks.push_back(p1Add);
    }
    else
    {
      std::cout << "Skipped p1 track. Radius: " << rmin << std::endl;
    }

    if(!(p2Add == Surface::GlobalPoint()))
    {
      p2Tracks.push_back(p2Add);
    }
    else
    {
      std::cout << "Skipped p2 track. Radius: " << rmin << std::endl;
    }
    
    rmin -= 0.1;
  }

  double closestApproach = 100000;
  for(Surface::GlobalPoint p1Track : p1Tracks) {
    for(Surface::GlobalPoint p2Track : p2Tracks) {
      double approach = (p1Track - p2Track).mag();
      if(approach < closestApproach) {
        closestApproach = approach;
      }
    }
  }
  return closestApproach;
}

/*

void DisplacedVertexModule::muonBarrelCrossing(reco::TrackRef track) 
{
  auto& refPoint = track->referencePoint();
  auto& refMomentum = track->momentum();
  
  GlobalPoint trackPoint(refPoint.x(), refPoint.y(), refPoint.z());
  GlobalVector trackMomentum(refMomentum.x(), refMomentum.y(), refMomentum.z());

  double rho = 0.1;
  double s = 1;

  std::cout << "refPoint x: " << trackPoint.x() << " | momentum x: " << trackMomentum.x() << std::endl;
  
  HelixBarrelPlaneCrossingByCircle* helixCrossing = new HelixBarrelPlaneCrossingByCircle(trackPoint, trackMomentum, rho, alongMomentum);

  std::cout << "position: " << helixCrossing->position(s) << std::endl;
  std::cout << "direction: " << helixCrossing->direction(s) << std::endl;
    
}

// Test example
void DisplacedVertexModule::testCrossing() 
{
  GlobalPoint startingPos(-7.79082, -47.6418, 9.18163);
  GlobalVector startingDir(-0.553982, -5.09198, 1.02212);

  double rho = 0.00223254;

  HelixBarrelPlaneCrossingByCircle* precise =  new HelixBarrelPlaneCrossingByCircle(startingPos, startingDir, rho, alongMomentum);
  //bool cross;
  double s;
  //std::tie(cross, s) = precise->pathLength(plane);

  s = 10.5;
  std::cout << "position: " << precise->position(s) << std::endl;


  
  GlobalPoint pos(-2.95456, -48.2127, 3.1033);

  Surface::RotationType rot(0.995292, 0.0969201, 0.000255868, 8.57131e-06, 0.00255196, -0.999997, -0.0969205, 0.995289, 0.00253912);

  std::cout << rot << std::endl;

  Plane plane(pos, rot);
  GlobalVector u = plane.normalVector();
  std::cout << "norm " << u << std::endl;
  
  auto crossing = new HelixBarrelPlaneCrossing2OrderLocal(startingPos, startingDir, rho, plane);

  std::cout << plane.toLocal(GlobalPoint(precise.position(s))) << " " << plane.toLocal(GlobalVector(precise.direction(s))) << std::endl;
  std::cout << HelixBarrelPlaneCrossing2OrderLocal::positionOnly(startingPos, startingDir, rho, plane) << ' ';
  std::cout << crossing.position() << ' ' << crossing.direction() << std::endl;
 }
 */

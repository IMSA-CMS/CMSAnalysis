#include "CIAnalysis/CIStudies/interface/DisplacedVertexModule.hh"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "MagneticField/UniformEngine/interface/UniformMagneticField.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include <limits>

#include <algorithm>
#include <cmath>
#include <tuple>
#include <cfloat>

#include <iostream> 

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
  std::vector<Particle> recoCandidates(getInput()->getParticles(InputModule::RecoLevel::Reco).getParticles());

  for (auto& part : recoCandidates)
  {
    auto particle = part.getUnderlyingParticle();
    
    auto muon = dynamic_cast<const pat::Muon*>(particle);

    if (muon)
    {
      auto muonTrack = muon->track();
      // auto muonTrack = muon->outerTrack();

      if (!muonTrack) 
      {
        std::cout << "NO TRACK" << std::endl;
      }
      else
      {
        propagateTrack(muonTrack);
      }
    }
    else
    {
      std::cout << "NOT A MUON" << std::endl;
    }
  }

  return true; 
}

void DisplacedVertexModule::propagateTrack(reco::TrackRef track)
{
  Surface::GlobalVector magneticFieldVector(0, 0, 4); // wrong magnetic field
  
  // Everything seems to need a regular Magnetic Field, but that's an abstract class?
  const UniformMagneticField* magneticField = new UniformMagneticField(magneticFieldVector);

  /*
  auto& outerPoint = track->outerPosition(); // should I be using this or referencePosition?
  auto& outerMomentum = track->outerMomentum();

  Surface::GlobalPoint trackPoint(outerPoint.x(), outerPoint.y(), outerPoint.z());
  Surface::GlobalVector trackMomentum(outerMomentum.x(), outerMomentum.y(), outerMomentum.z());
  */

  auto& innerPoint = track->innerPosition(); // should I be using this or referencePosition?
  auto& innerMomentum = track->innerMomentum();

  Surface::GlobalPoint trackPoint(innerPoint.x(), innerPoint.y(), innerPoint.z());
  Surface::GlobalVector trackMomentum(innerMomentum.x(), innerMomentum.y(), innerMomentum.z());

  std::cout << "INNER POINT --> " << innerPoint.x() << " " << innerPoint.y() << std::endl;

  TrackCharge trackCharge = 1; // I assume this or -1?

  FreeTrajectoryState* muonTrajectory = new FreeTrajectoryState(trackPoint, trackMomentum, trackCharge, magneticField);
  std::cout << "fts X --> " << muonTrajectory->position().x() << std::endl;

  AnalyticalPropagator* propagator = new AnalyticalPropagator(magneticField, oppositeToMomentum);

  const Cylinder::PositionType cylinderPos(0, 0, 0);
  const Cylinder::RotationType cylinderRot(1, 0, 0, 0, 1, 0, 0, 0, 1); // fix these, these types are confusing
  
  float rmin = 2;
  float rmax = 2.2;
  float zmin = -50;
  float zmax = 50;

  SimpleCylinderBounds cylinderBounds(rmin, rmax, zmin, zmax);
  Cylinder barrelCylinder(cylinderPos, cylinderRot, cylinderBounds);

  BasicSingleTrajectoryState* basicTrajectory = new BasicSingleTrajectoryState(*muonTrajectory, barrelCylinder);
  TrajectoryStateOnSurface* trajectoryOnSurface = new TrajectoryStateOnSurface(basicTrajectory);

  std::pair<TrajectoryStateOnSurface, double> propagateWithPathTSOS = propagator->propagateWithPath(*trajectoryOnSurface, barrelCylinder);

  // std::cout << "TSOS Position --> " << propagateWithPathTSOS.second << std::endl;

  std::cout << "TSOS Position --> " << propagateWithPathTSOS.first.globalPosition().x() << " " << propagateWithPathTSOS.first.globalPosition().y() << std::endl; // seg faults
  // std::cout << "TSOS Position --> " << propagateWithPathTSOS.first.data() << std::endl;
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
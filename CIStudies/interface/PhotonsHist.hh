#ifndef PHOTONSHIST_HH
#define PHOTONSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
#include "CIAnalysis/CIStudies/interface/Particle.hh"
class ParticleCollection;


class PhotonsHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
  private:
  void addPhotonUsingIM (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingDR (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingPt (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingPhi (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingEta (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingIMIs800 (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
  void addPhotonUsingMother (ParticleCollection& pc1, ParticleCollection& pc2, Particle photon) const;
};

#endif


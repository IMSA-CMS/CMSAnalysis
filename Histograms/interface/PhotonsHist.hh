#ifndef PHOTONSHIST_HH
#define PHOTONSHIST_HH

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"


class PhotonsHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    std::vector<double> protectedValue(InputModule::RecoLevel typeGenSim) const override;
  private:
  void addPhotonUsingIM (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingDR (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingPt (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingPhi (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingEta (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingIMIs800 (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const;
  void addPhotonUsingMother (ParticleCollection<GenSimParticle>& pc1, ParticleCollection<GenSimParticle>& pc2, GenSimParticle& photon) const;
};

#endif


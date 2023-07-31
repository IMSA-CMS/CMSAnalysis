#include "CMSAnalysis/Histograms/interface/PhotonElectronInvariantMassHist.hh"

PhotonElectronInvariantMassHist::PhotonElectronInvariantMassHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum)
{
}

std::vector<double> PhotonElectronInvariantMassHist::protectedValue(EventInput::RecoLevel typeGenSim) const
{
    auto photons = getInput()->getParticles(typeGenSim, ParticleType::photon());
    auto electrons = getInput()->getParticles(typeGenSim, ParticleType::electron());
    double max = -10000000;
    // find the max 
    for (auto photon : photons)
    {
        for (auto electron : electrons)
        {
            ParticleCollection<Particle> coll;
            coll.addParticle(photon);
            coll.addParticle(electron);
            auto invMass = coll.calculateAllLeptonInvariantMass();
            if (invMass > max)
            {
                max = invMass;
            }
        }
    }
    return {max};
}

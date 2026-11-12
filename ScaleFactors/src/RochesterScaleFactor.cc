#include "CMSAnalysis/ScaleFactors/interface/RochesterScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"  
#include "CMSAnalysis/Filters/interface/RoccoR.h"   

RochesterScaleFactor::RochesterScaleFactor(std::string iname, std::string correctionFile)
    : ScaleFactor(iname),                                
      roccoR_(std::make_unique<RoccoR>(correctionFile)),
      rng(12345)
{
}

RochesterScaleFactor::~RochesterScaleFactor() = default;

double RochesterScaleFactor::getScaleFactor(const EventInput* input, SystematicType type) const
{
    int s = 0;
    int m = 0;

    const bool isData = input->isData();
    auto muons = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon(), false); 
    auto genParticles = input->getParticles(EventInput::RecoLevel::GenSim);

    double product = 1.0;

    for (const auto& mu : muons)
    {
        double charge = mu.getCharge();   // Muon_charge
        double pt     = mu.getPt();       // Muon_pt
        double eta    = mu.getEta();      // Muon_eta
        double phi    = mu.getPhi();      // Muon_phi
        int nLayers   = mu.getInfo("nTrackerLayers"); // Muon_nTrackerLayers

        double k = 1.0;

        if (isData)
        {
            k = roccoR_->kScaleDT(charge, pt, eta, phi, s, m);
        }
        else
        {
            int genIdx = mu.getInfo("genIdx");   // Muon_genPartIdx, -1 if no match
            if (genIdx >= 0 && genIdx < static_cast<int>(genParticles.size()))
            {
                double genPt = genParticles[genIdx].getPt(); // GenPart_pt[genIdx]
                k = roccoR_->kSpreadMC(charge, pt, eta, phi, genPt, s, m);
            }
            else
            {
                double u = rng.Rndm();
                k = roccoR_->kSmearMC(charge, pt, eta, phi, nLayers, u, s, m);
            }
        }

        product *= k;
    }

    return product;
}

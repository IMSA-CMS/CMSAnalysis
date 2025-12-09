#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include <vector>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

double HiggsSelector:: massDifference(const std::vector<Particle>& positiveLeptons, const std::vector<Particle>& negativeLeptons) const
{
    if (positiveLeptons.size() != 2 || negativeLeptons.size() != 2)
    {
        throw std::invalid_argument("massDifference requires exactly 2 positive and 2 negative leptons.");
    }

    double massPos = Particle::invariantMass(positiveLeptons[0], positiveLeptons[1]);
    double massNeg = Particle::invariantMass(negativeLeptons[0], negativeLeptons[1]);

    return std::abs(massPos - massNeg);
}

 void HiggsSelector::selectParticles(const EventInput* input, Event& event) const
{
    auto particles = input->getLeptons(EventInput::RecoLevel::Reco).getParticles();


    std::vector<Particle> leptons;


    for (const auto& particle : particles)
    {
        if (particle.getType() == ParticleType::electron())
     {
            auto lepton = Lepton(particle);
            if(lepton.isLoose()
            && particle.getPt() > 10
               
            )
            {
                //std::cout << "PT: " << std::to_string(particle.getPt()) << std::endl;
                leptons.push_back(particle);
            }
        }
        else if (particle.getType() == ParticleType::muon())
        {
            //std::cout << "In Muon Selection" << std::endl;
            auto lepton = Lepton(particle);
            if (lepton.isTight()  
                && particle.getPt() > 5
                // && particle.getInfo("Isolation") < 0.05
                // && lepton.getDXY() < 0.025
                // && lepton.getDZ() < 0.05
            )
            {
                leptons.push_back(particle);
            }
        }
        
        else if (particle.getType() == ParticleType::tau())
        {
            auto lepton = Lepton(particle);
            //need another loop over particle. if its a tau loop over all particles again, if one of the particles is an election or muon, then you want to calculate delta R for each particle. There's already a function for it (Particle.hh). If the delta r is less than some value we set, ten we just say that they're the same particle. If number is too small just continue. 
            if (lepton.isTight() && particle.getPt() > 5)
            {
                bool overlap = false;

                // loop over all other particles (from input, not just taus)
                for (const auto& other : particles)
                {
                    if (other == particle) continue; // skip self

                    // only check against electrons and muons
                    if (other.getType() == ParticleType::electron() ||
                        other.getType() == ParticleType::muon())
                    {
                        double dR = particle.getDeltaR(other); // Particle.hh already has this
                        if (dR < 0.3) // typical cone cut, can tune to 0.2–0.4
                        {
                            overlap = true;
                            break;
                        }
                    }
                }

                if (!overlap)
                {
                    leptons.push_back(particle); // only keep tau if no e/μ is too close
                }
            }
        }

    }


    while (leptons.size() > 4)
    {
        std::vector<double> pTs;
        for (auto particle : leptons)
        {
            pTs.push_back(particle.getPt());
        }
        double minPt = *min_element(pTs.begin(), pTs.end());
        int index = find(pTs.begin(), pTs.end(), minPt) - pTs.begin();
        leptons.erase(leptons.begin() + index);
    }

    auto correctedLeptons = adjustForNeutrinos(leptons, input);

    for (auto particle : leptons)
    {
        if (particle.getType() == ParticleType::electron())
        {
            event.addElectron(particle);
        }
        else if (particle.getType() == ParticleType::muon())
        {
            //std::cout << particle.getInfo("Isolation") << std::endl;
            event.addMuon(particle);
        }
        else if (particle.getType() == ParticleType::tau())
        {
            //std::cout << particle.getInfo("Isolation") << std::endl;
            event.addTau(particle);
        }
    }
}

std::vector<Particle> HiggsSelector::adjustForNeutrinos(const std::vector<Particle>& positiveLeptons, const std::vector<Particle>& negativeLeptons, const EventInput* input) const
{
    double smallestMassDiff = std::numeric_limits<double>::max();
    std::vector<Particle> bestLeptons;

    // Combine all leptons first
    std::vector<Particle> leptons;
    leptons.insert(leptons.end(), positiveLeptons.begin(), positiveLeptons.end());
    leptons.insert(leptons.end(), negativeLeptons.begin(), negativeLeptons.end());

    if (leptons.size() != 4)
    {
        std::cout << "Not 4 leptons → skipping neutrino reconstruction.\n";
        return leptons;
    }

    const double met_x = input->getMET_x();
    const double met_y = input->getMET_y();

    //first step is zero neutrinos (just as they are)
    {
        Particle total = leptons[0] + leptons[1] + leptons[2] + leptons[3];
        double mass = total.mass();
        double diff = massDifference(positiveLeptons, negativeLeptons);

        std::cout << "Case 0 (no neutrinos): mass = "
                  << mass << " diff = " << diff << std::endl;

        if (diff < smallestMassDiff)
        {
            smallestMassDiff = diff;
            bestLeptons = leptons;
        }
    }

    //next step is one neutrino (each gets a trial, each lepton gets a neutrino)
    {
        for (int i = 0; i < 4; i++)
        {
            std::vector<Particle> trial = leptons;

            // Neutrino from MET (pz=0 simple approximation)
            double nu_px = met_x;
            double nu_py = met_y;
            double nu_pT  = std::sqrt(nu_px*nu_px + nu_py*nu_py);

            double scale = nu_pT / trial[i].getPt();
            double newFourVector = trial[i].getFourVector() * (scale + 1);

            // Add neutrino to this lepton
            trial[i] = Particle(newFourVector, trial[i].getCharge(), trial[i].getType(), trial[i].getSelectionFit());
            double diff = massDifference(positiveLeptons, negativeLeptons);

            std::cout << "Case 1: neutrino assigned to lepton "
                      << i << " mass = " << mass << " diff = " << diff << std::endl;

            if (diff < smallestMassDiff)
            {
                smallestMassDiff = diff;
                bestLeptons = trial;
            }
        }
    }

    //then two neutrinos (each unique pair of leptons gets a trial)
    {
        int pairs[6][2] =
        {
            {0,1}, {0,2}, {0,3},
            {1,2}, {1,3},
            {2,3}
        };

        for (int p = 0; p < 6; p++)
        {
            int i = pairs[p][0];
            int j = pairs[p][1];

            // Transverse momentum components
            double pix = leptons[i].px();
            double piy = leptons[i].py();
            double pjx = leptons[j].px();
            double pjy = leptons[j].py();

            // Solve:
            // met_x = a*pix + b*pjx
            // met_y = a*piy + b*pjy

            double det = pix * pjy - piy * pjx;
            if (std::abs(det) < 1e-6)
            {
                std::cout << "Case 2: Pair (" << i << "," << j
                          << ") DET=0, skipping.\n";
                continue;
            }

            double a = ( met_x * pjy - met_y * pjx ) / det;
            double b = (-met_x * piy + met_y * pix ) / det;

            std::cout << "Case 2: Pair (" << i << "," << j
                      << ") a=" << a << " b=" << b << std::endl;


            auto lep_i_new4 = leptons[i].getFourVector() * (1.0 + a);
            auto lep_j_new4 = leptons[j].getFourVector() * (1.0 + b);

            std::vector<Particle> trial = leptons;
            trial[i] = Particle(lep_i_new4, leptons[i].getCharge(), leptons[i].getType(), leptons[i].getSelectionFit());
            trial[j] = Particle(lep_j_new4, leptons[j].getCharge(), leptons[j].getType(), leptons[j].getSelectionFit());

            double mass = total.mass();
            double diff = massDifference(positiveLeptons, negativeLeptons);

            std::cout << "Case 2: Pair (" << i << "," << j
                      << ")  mass = " << mass
                      << " diff = " << diff << std::endl;

            if (diff < smallestMassDiff)
            {
                smallestMassDiff = diff;
                bestLeptons = trial;
            }
        }
    }

    return bestLeptons;
}
    
    // if (Leptons.size() != 4)
    //     return leptons; // only adjust if exactly 4 leptons

//         struct Vector2D {
//         double x;
//         double y;

//         Vector2D operator+(const Vector2D& other) const {
//             return {x + other.x, y + other.y};
//         }

//         Vector2D operator*(double scalar) const {
//             return {scalar * x, scalar * y};
//         }
//     };

//     Vector2D p1T { p1.px(), p1.py() };
//     Vector2D p2T { p2.px(), p2.py() };
//     Vector2D MET { met_x, met_y };

//     std::cout << "p1T = (" << p1T.x << ", " << p1T.y << ")\n";
//     std::cout << "p2T = (" << p2T.x << ", " << p2T.y << ")\n";
//     std::cout << "MET = (" << MET.x << ", " << MET.y << ")\n";
    
//         double met_x = input->getMET_x();
//         double met_y = input->getMET_y();
//         double MET = std::sqrt(met_x*met_x + met_y*met_y);

//         if (MET < METThreshold_) {
//             std::cout << "MET below threshold — skipping neutrino adjustment.\n";
//             return leptons;
//         }

//         for (int mask = 0; mask < 16; mask++)
//         {
//             int bitCount = __builtin_popcount(mask);
        
//         if (bitCount > 2)
//             continue;
        
//         std::array<double, 4> alpha{};
        
//         for (int i = 0; i < 4; i++)
//         {
//             alpha[i] = (mask & (1 << i)) ? 1.0 : 0.0;
//         };
        
//         for (auto& alpha : trials)
//         {
        
//             std::vector<Particle> scaled;
//             scaled.reserve(4);
//             for (int i = 0; i < 4; i++)
//                 scaled.push_back(leptons[i].scale(1.0 - alpha[i]));
//                 // <-- implement scale() or write inlin
            
//             double mA = reconstructMass(scaled[0], scaled[1]);
//             double mB = reconstructMass(scaled[2], scaled[3]);
//             double diff = std::fabs(mA - mB);
            
//             if (diff < bestDiff)
//             {
//                 bestDiff = diff;
//                 bestAlpha = alpha;
//             }
//         }
// }
        
    

 



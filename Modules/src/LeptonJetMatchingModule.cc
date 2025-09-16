#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include <iostream>

int numOfDarkPhotons = 0;
int numOfPhotons = 0;
int numOfElectronMatches = 0;
int numOfMuonMatches = 0;
int noMatch = 0;
int numOfPhoton_Electron_Matches = 0;
int numOfPhoton_Muon_Matches = 0;
int numOfElectron_Muon_Matches = 0;
int numOfMuon_Electron_Matches = 0;

LeptonJetMatchingModule::LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule,
                                                 double deltaRCut)
    : lepJet(lepJetModule), deltaRCutoff(deltaRCut)
{
    addRequiredModule(lepJetModule);
}

bool LeptonJetMatchingModule::process()
{
    auto darkPhotons(
        getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::darkPhoton()).getParticles());
    auto recoParticles(getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::none()).getParticles());
    auto genSim(getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none()).getParticles());

    MatchingModule::match(genSim, recoParticles);

    const MatchingPairCollection &bestLeptonPairs = getMatchingBestPairs();
    const std::vector<Particle> recoLeptons = bestLeptonPairs.getRecoParticles().getParticles();
    const std::vector<GenSimParticle> underLepton = bestLeptonPairs.getGenParticles().getParticles();

    std::vector<Particle> underlyingLepton;
    for (Particle const &lepton : underLepton)
    {
        if (lepton.getPt() >= 5 &&
            (lepton.getType() == ParticleType::electron() || lepton.getType() == ParticleType::muon()))
        {
            underlyingLepton.push_back(lepton);
        }
    }

    genLeptons += underlyingLepton.size();

    for (GenSimParticle lepton : underlyingLepton)
    {
        if (!lepton.isFinalState() || lepton.getPt() < 5)
        {
            continue;
        }
        bool passedDarkPhoton = false;
        GenSimParticle particle = lepton;
        bool forgetIt = false;
        while (!(particle.getType().isQuark() || particle.getType() == ParticleType::higgs() || particle.status() == 4))
        {
            if (particle.hasUniqueMother() == false)
            {
                break;
            }

            try
            {
                particle = particle.uniqueMother();
                //particle = particle.mother();
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception caught: " << e.what() << std::endl << "\n";
                continue;
            }
            //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;

            if (particle.pdgId() == 0)
            {
                forgetIt = true;
                break;
            }

            if (particle.getType() != ParticleType::darkPhoton())
            {
                continue;
            }

            passedDarkPhoton = true;
            numOfDarkPhotons++;
            //std::cout<<"The particle's type is: "<<particle.getType().getName() << "\n";

            //do delta r comparison
            double minDeltaR = 500;
            GenSimParticle closestMatch = lepton;

            for (auto &newParticle : recoLeptons)
            {
                if (lepton.getDeltaR(newParticle) < 0.1 && lepton.getDeltaR(newParticle) < minDeltaR)
                {
                    minDeltaR = lepton.getDeltaR(newParticle);
                    closestMatch = newParticle;
                }
            }

            if (closestMatch.getType() == lepton.getType())
            {
                if (closestMatch.getType() == ParticleType::muon())
                {
                    //std::cout << "The closest match is: " << closestMatch.getType().getName() << " and the lepton is: " << lepton.getType().getName() << "\n";
                    numOfMuonMatches++;
                    //std::cout << "The num of muon matches is: " << numOfMuonMatches << "\n\n";
                }
                else if (closestMatch.getType() == ParticleType::electron())
                {
                    //std::cout <<"The closest match is: " << closestMatch.getType().getName() << " and the lepton is: " << lepton.getType().getName() << "\n";
                    numOfElectronMatches++;
                    //std::cout << "The num of electron matches is: " << numOfElectronMatches << "\n\n";
                }
                else
                {
                    std::cout << "The closest match is: " << closestMatch.getType().getName()
                              << " and the lepton is: " << lepton.getType().getName() << "\n";
                }
            }
            else
            {
                noMatch++;
                if (closestMatch.getType() == ParticleType::photon() && lepton.getType() == ParticleType::electron())
                {
                    numOfPhoton_Electron_Matches++;
                    //std::cout<<"No match between closest match " << closestMatch.getType().getName() << " and  lepton " << lepton.getType().getName() << "\n";
                }
                else if (closestMatch.getType() == ParticleType::photon() && lepton.getType() == ParticleType::muon())
                {
                    numOfPhoton_Muon_Matches++;
                }
                else if (closestMatch.getType() == ParticleType::electron() && lepton.getType() == ParticleType::muon())
                {
                    numOfElectron_Muon_Matches++;
                }
                else if (closestMatch.getType() == ParticleType::muon() && lepton.getType() == ParticleType::electron())
                {
                    numOfMuon_Electron_Matches++;
                }
                else
                {
                    std::cout << "No match: The closest match: " << closestMatch.getType().getName()
                              << " Lepton: " << lepton.getType().getName() << "\n";
                }
            }
        }

        if (forgetIt)
        {
            break;
        }
        if (particle.getType().isQuark())
        {
            quark++;
        }

        if (particle.getType() == ParticleType::higgs())
        {
            if (passedDarkPhoton)
            {
                darkPhotonOrigin++;
            }
        }

        if (particle.getType() == ParticleType::photon())
        {
            numOfPhotons++;
            //std::cout<<"photon\n";
        }

        if (particle.status() == 4)
        {
            proton++;
        }
    }

    for (Particle const &darkPhoton : darkPhotons)
    {
        if (darkPhoton.getPt() >= 10 && abs(darkPhoton.getEta()) <= 3)
        {
            genSize++;
        }
    }

    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());

    std::vector<Particle> lJets;
    for (LeptonJet const &lJet : recoLeptonJets)
    {
        lJets.push_back(lJet);
    }

    bool success = MatchingModule::match(darkPhotons, lJets);
    if (success)
    {
        // return false;
    }
    std::vector<MatchingPair> matchedLeptonJets = getMatchingBestPairs().getPairs();

    // std::cout << "Dark photons: " << darkPhotons.size() << "\n";
    // for (auto const &gammaD : darkPhotons)
    // {
    //     std::cout << "    " << gammaD << "\n";
    // }
    // std::cout << "Lepton jets: " << recoLeptonJets.size() << "\n";
    // for (auto const &jet : recoLeptonJets)
    // {
    //     std::cout << "    Ljet particles:\n";
    //     for (auto const &lJetParticle : jet.getParticles())
    //     {
    //         std::cout << "        " << lJetParticle << "\n";
    //     }
    // }
    // std::cout << "Matches: " << matchedLeptonJets.size() << "\n";
    // for (MatchingPair const &pair : matchedLeptonJets)
    // {
    //     std::cout << "    Dark photon: " << pair.getGenParticle() << "\n";
    //     std::cout << "    Ljet particles:\n";
    //     for (auto const &lJetParticle : LeptonJet(pair.getRecoParticle()).getParticles())
    //     {
    //         std::cout << "        " << lJetParticle << "\n";
    //     }
    // }

    lepJetSize += recoLeptonJets.size();
    darkPhoton += matchedLeptonJets.size();

    return true;
}

void LeptonJetMatchingModule::finalize()
{
    std::cout << "Lepton Jets Found: " << lepJetSize << "\n";
    std::cout << "Number of Dark Photons: " << genSize << "\n";
    std::cout << "Lepton Jet Matching Efficiency: " << (double)darkPhoton / genSize << "\n";
    std::cout << "Leptons From Quarks " << (double)quark / genLeptons << "\n";
    std::cout << "Leptons From Dark Photons " << (double)darkPhotonOrigin / genLeptons << "\n";
    std::cout << "Leptons From Squarks " << (double)squark / genLeptons << "\n";
    std::cout << "Leptons From Protons " << (double)proton / genLeptons << "\n";
    std::cout << "Number of Muon Matches " << numOfMuonMatches << "\n";
    std::cout << "Number of Electron Matches " << numOfElectronMatches << "\n";
    std::cout << "Number of No Matches " << noMatch << "\n";
    std::cout << "Number of Photon Electron Pairings " << numOfPhoton_Electron_Matches << "\n";
    std::cout << "Number of Photon Muon Pairings " << numOfPhoton_Muon_Matches << "\n";
    std::cout << "Number of Muon Electron Pairings " << numOfMuon_Electron_Matches << "\n";
    std::cout << "Number of Electron Muon Pairings " << numOfElectron_Muon_Matches << "\n";
}

const bool LeptonJetMatchingModule::isQuark(GenSimParticle lepton) const
{
    if (abs(lepton.pdgId()) == 1 || abs(lepton.pdgId()) == 2 || abs(lepton.pdgId()) == 3 || abs(lepton.pdgId()) == 4)
    {
        //quark++;
        return true;
    }
    else if (abs(lepton.pdgId()) == 5 || abs(lepton.pdgId()) == 6)
    {
        //quark++;
        return true;
    }
    return false;
}
const bool LeptonJetMatchingModule::isSquark(GenSimParticle lepton) const
{
    if (abs(lepton.pdgId()) == 1000001 || abs(lepton.pdgId()) == 1000002 || abs(lepton.pdgId()) == 1000003 ||
        abs(lepton.pdgId()) == 1000004)
    {
        //squark++;
        return true;
    }
    else if (abs(lepton.pdgId()) == 1000005 || abs(lepton.pdgId()) == 1000006)
    {
        //squark++;
        return true;
    }
    return false;
}
const bool LeptonJetMatchingModule::isHiggs(GenSimParticle lepton) const
{
    if (abs(lepton.pdgId()) == 25)
    {
        return true;
    }
    return false;
}

#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "DataFormats/Math/interface/deltaR.h"

#include <limits>

LeptonJetMatchingModule::LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut) :
  //genSim(genSimModule),
  lepJet(lepJetModule),
  deltaRCutoff(deltaRCut)

{}

bool LeptonJetMatchingModule::process()
{
    matchingPairs.clear();
    std::vector<Particle> genSimParticles(getInput()->getParticles(InputModule::RecoLevel::GenSim).getParticles());
    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());

    genSize += genSimParticles.size();
    // std::cout << "Size of genSimParticles: " << genSimParticles.size() << "\n";
    lepJetSize += recoLeptonJets.size();
    // std::cout << "Size of recoLeptonJets: " << recoLeptonJets.size() << "\n";

    double deltaRMin;
    // MatchingPair candidate;
    Particle nullParticle(nullptr);
    LeptonJet nullJet(nullParticle);

    while (genSimParticles.size() != 0 && recoLeptonJets.size() != 0)
    {
        deltaRMin = std::numeric_limits<double>::max(); // Highest possible double, update after every matched pair
        // MatchingPair dataList(nullptr, nullptr);
        MatchingPair dataList(nullParticle, nullJet);
        for (auto &genParticle : genSimParticles)
        {
          // candidate.first = genParticle;
          for (auto &recoJet : recoLeptonJets)
          {
            MatchingPair candidate(genParticle, recoJet);
            // candidate.second = recoJet;
            double pairDeltaR = findMatchingPairDeltaR(candidate);

            if (pairDeltaR < deltaRMin)
            {
              dataList = candidate; // dataList is needed because candidate is not defined outside the for-each loops
              deltaRMin = pairDeltaR;
            }
          }
        }
        if (deltaRMin < deltaRCutoff)
        {
          //std::cout << "Match found!" << "\n";
          matchingPairs.push_back(dataList);
          recoLeptonJets.erase(std::find(recoLeptonJets.begin(), recoLeptonJets.end(), dataList.second));
        }
        genSimParticles.erase(std::find(genSimParticles.begin(), genSimParticles.end(), dataList.first));
    }
    return true;
}

double LeptonJetMatchingModule::findMatchingPairDeltaR(MatchingPair pair)
{
  Particle part = pair.first;
  LeptonJet jet = pair.second;

  auto partFourVector = part.fourVector();
  auto jetFourVector = jet.getFourVector();

  double deltaR = reco::deltaR(partFourVector, jetFourVector);

  return deltaR;
}

void LeptonJetMatchingModule::finalize()
{
  std::cout << "Lepton Jets Found: " << lepJetSize << "\n";
  std::cout << "Number of Neutralinos: " << genSize << "\n";
  std::cout << "Lepton Jet Matching Efficiency: " << (double) lepJetSize / genSize << "\n";
}

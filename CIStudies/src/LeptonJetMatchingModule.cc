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
    std::vector<Particle> lJets;
    foreach(LeptonJet lJet:recoLeptonJets)
    {
      lJets.push_back(lJet);
    }

    genSize += genSimParticles.size();
    // std::cout << "Size of genSimParticles: " << genSimParticles.size() << "\n";
    lepJetSize += recoLeptonJets.size();
    // std::cout << "Size of recoLeptonJets: " << recoLeptonJets.size() << "\n";

    double deltaRMin;
    // MatchingPair candidate;
    Particle nullParticle(nullptr);
    LeptonJet nullJet(nullParticle);

    return match(genSimParticles, lJets);
}

double LeptonJetMatchingModule::findMatchingPairDeltaR(MatchingPair pair)
{
  Particle part = pair.first;
  LeptonJet jet = pair.second;

  auto partFourVector = part.getFourVector();
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

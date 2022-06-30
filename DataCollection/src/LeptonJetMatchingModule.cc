#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
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
    std::vector<Lepton> temp(getInput()->getLeptons(InputModule::RecoLevel::GenSim).getParticles());
    std::vector<Particle> genSimParticles(temp.begin(), temp.end());
    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());
    std::vector<Particle> lJets;
    for (LeptonJet lJet:recoLeptonJets)
    {
      lJets.push_back(lJet);
      //std::cout << "LJet Matching: lJet list size:" << lJets.size() << "\n";
    }
    //std::cout << "Size of genSimParticles: " << genSimParticles.size() << "\n";
    //std::cout << "Size of recoLeptonJets: " << recoLeptonJets.size() << "\n";
    MatchingModule::match(genSimParticles, lJets);
    lepJetSize += getMatchingPairs().size();
    genSize += genSimParticles.size();
    lepJetSize += recoLeptonJets.size();

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

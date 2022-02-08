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
    std::vector<Particle> genSimParticles(getInput()->getParticles(InputModule::RecoLevel::GenSim, Particle::Type::DarkPhoton).getParticles());
    //std::cout << "Ljet matching (GenSim).\n";
    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());
    std::vector<Particle> lJets;
    for (LeptonJet lJet:recoLeptonJets)
    {
      lJets.push_back(lJet);
      std::cout << "LJet Matching: lJet list size:" << lJets.size() << "\n";
    }
    MatchingModule::match(genSimParticles, lJets);
    lepJetSize += getMatchingPairs().size();
    genSize += genSimParticles.size();
    return true;
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

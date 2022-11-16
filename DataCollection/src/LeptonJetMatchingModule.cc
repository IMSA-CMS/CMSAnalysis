#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"

#include <limits>
#include <fstream>

LeptonJetMatchingModule::LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, std::shared_ptr<GenSimEventDumpModule> dump, double deltaRCut) :
  //genSim(genSimModule),
  lepJet(lepJetModule),
  eventDump(dump),
  deltaRCutoff(deltaRCut)
{
  
}
bool latch = true; 
bool LeptonJetMatchingModule::process()
{
    matchingPairs.clear();
    auto darkPhotons(getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton()).getParticles());
    auto genSimParticles(getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::none()).getParticles());

    genSize += darkPhotons.size();
    
    //std::vector<Particle> genSimParticles(temp.begin(), temp.end());
    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());
    std::vector<Particle> lJets;
    for (LeptonJet& lJet:recoLeptonJets)
    {
      //std::cout<<"Lepton Jet "<<lJet<<std::endl;
      // auto lJetParticle = lJet.getParticles();
      // for(auto particle: lJetParticle){
      //   std::cout<<"The Particle is not null "<<particle.getName()<<std::endl;
      // }
      //std::cout<<"The Lepton Jet null "<<lJet.isNotNull()<<std::endl;
      //std::cout<<lJet.getName()<<std::endl;
      //std::cout<<"Making Particle"<<Particle(lJet).isNotNull()<<std::endl;
      lJets.push_back(lJet);
      //std::cout << "LJet Matching: lJet list size:" << lJets.size() << "\n";
    }
    //std::cout << "Size of genSimParticles: " << genSimParticles.size() << "\n";
    //std::cout << "Size of recoLeptonJets: " << lJets.size() << "\n";

    //MatchingModule::match(genSimParticles, lJets);
    MatchingModule::match(genSimParticles, lJets);
    //std::cout << "Size of matchingPairs: " << getMatchingBestPairs().getSize()<<"\n";
    lepJetSize += getMatchingBestPairs().getSize();
    std::vector<MatchingPair> matchedLeptonJets = getMatchingPairs();
    //std::cout<<getMatchingPairs().size()<<std::endl;
    for (MatchingPair leptonPair : matchedLeptonJets)
    {
      //std::cout<<leptonPair.first.getName()<<std::endl;
      if(!(leptonPair.first.getType() == ParticleType::darkPhoton()))
      {
        // //std::cout<<"Made it to loop"<<std::endl;
        // std::ofstream my_file;
        // if (latch) 
        // {
        //   my_file.open("leptonJet.txt", std::ofstream::out | std::ofstream::trunc);
        //   latch = false;
        // }
        // //std::cout<<"The origin particle is "<<leptonPair.first.getName()<<std::endl;
        // ParticleCollection<Particle> leptonJet;
        // leptonJet.addParticle(leptonPair.first);
        // for (Particle lepton: leptonPair.second.getParticles())
        // {
        //   leptonJet.addParticle(lepton);
        // }
        // //ParticleCollection<Particle> leptonJet = leptonPair.second.getParticle();
        // my_file.open("leptonJet.txt", std::ios::app);
        // //std::cout<<"The Size of Particle Collection "<<leptonJet.getNumParticles()<<std::endl;
        // eventDump->printGenSimParticleCollection(leptonJet, my_file);
        // my_file.close();
      }
      else
      {
        darkPhoton++;
      }
    }
    
    //lepJetSize += recoLeptonJets.size();

    // MatchingPair candidate;
    Particle nullParticle(Particle::nullParticle());
    LeptonJet nullJet(nullParticle);

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
  std::cout << "Number of Dark Photons: " << genSize << "\n";
  std::cout << "Lepton Jet Matching Efficiency: " << (double) darkPhoton / genSize << "\n";
}

const std::vector<std::pair<Particle,LeptonJet>> LeptonJetMatchingModule::getMatchingPairs() const {
  auto& matchedParticles = getMatchingBestPairs().getPairs();
  std::vector<MatchingPair> matchedLeptonJets;
  for (auto& matched : matchedParticles){
    matchedLeptonJets.push_back(std::make_pair(matched.getGenParticle(),matched.getRecoParticle())); //for some reason lepton jet is assigned as recoParticle in MatchingPair
  }
  return matchedLeptonJets;
}

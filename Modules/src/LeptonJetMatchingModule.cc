#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "DataFormats/Math/interface/deltaR.h"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"

#include <limits>
#include <fstream>

int numOfDarkPhotons = 0;
int numOfPhotons = 0;
int numOfElectronMatches = 0;
int numOfMuonMatches = 0;
int noMatch = 0;
int numOfPhoton_Electron_Matches = 0;
int numOfPhoton_Muon_Matches = 0;
int numOfElectron_Muon_Matches = 0;
int numOfMuon_Electron_Matches = 0;

LeptonJetMatchingModule::LeptonJetMatchingModule(std::shared_ptr<LeptonJetReconstructionModule> lepJetModule, double deltaRCut) :
  //genSim(genSimModule),
  lepJet(lepJetModule),
  deltaRCutoff(deltaRCut)
{
  addRequiredModule(lepJetModule);
}
bool latch = true; 
bool LeptonJetMatchingModule::process()
{
    static int counter = 0;
    //std::cout<<"the counter: " << counter <<"\n";
    counter++;
    matchingPairs.clear();
    auto darkPhotons(getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::darkPhoton()).getParticles());
    auto recoParticles(getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::none()).getParticles());
    auto genSim(getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none()).getParticles());

    //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;
    MatchingModule::match(genSim, recoParticles);

    const MatchingPairCollection& bestLeptonPairs = getMatchingBestPairs();
    const std::vector<GenSimParticle> underLepton = bestLeptonPairs.getGenParticles().getParticles();
    const std::vector<Particle> recoLeptons = bestLeptonPairs.getRecoParticles().getParticles();
    //std::cout<< __LINE__ << std::endl;
    std::vector<Particle> underlyingLepton;

    for (Particle lepton : underLepton)
    {
      if (lepton.getPt() >= 5 && (lepton.getType() == ParticleType::electron() || lepton.getType() == ParticleType::muon()))
      {
        underlyingLepton.push_back(lepton);
      }
    }
    //std::cout<< __LINE__ << std::endl;

    genLeptons += underlyingLepton.size();

    std::vector<std::string> particleParentage;

    std::vector<GenSimParticle> genSimVector;
    
    for(Particle genParticle: genSim)
    {
      genSimVector.push_back(GenSimParticle(genParticle));
    }

    //std::cout<< __LINE__ << std::endl;
    //std::vector<std::pair<GenSimParticle, GenSimParticle>> leptonOrigins;

    for(GenSimParticle lepton : underlyingLepton)
    {
      //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;
      if(!lepton.isFinalState() || lepton.getPt() < 5)
      {
        continue;
      }
      bool passedDarkPhoton = false;
      GenSimParticle particle = lepton;
      bool forgetIt = false;
      while(!(particle.getType().isQuark() || particle.getType() == ParticleType::higgs() || particle.status() == 4))
      {
        //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;
        if(particle.hasUniqueMother() == false) break;
        
        try{
        particle = particle.uniqueMother();
        //particle = particle.mother();
        }catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl<<"\n";
        //break;
        //throw;
        continue;
        }
        //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;

        if(particle.pdgId() == 0)
        {
          forgetIt = true;
          break;
        }

        if(particle.getType() == ParticleType::darkPhoton())
        {
          //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;
          passedDarkPhoton = true;
          numOfDarkPhotons++;
          //std::cout<<"The particle's type is: "<<particle.getType().getName() << "\n";

          //do delta r comparison
          double minDeltaR = 500;
          GenSimParticle closestMatch = lepton;

          for (auto &newParticle : recoLeptons)
          {
              if(lepton.getDeltaR(newParticle) < 0.1 && lepton.getDeltaR(newParticle) < minDeltaR)
              {
                minDeltaR = lepton.getDeltaR(newParticle);
                closestMatch = newParticle;
                if(closestMatch.getType() ==  lepton.getType())
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
                    std::cout <<"The closest match is: " << closestMatch.getType().getName() << " and the lepton is: " << lepton.getType().getName() << "\n";
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
                  else if(closestMatch.getType() == ParticleType::photon() && lepton.getType() == ParticleType::muon())
                  {
                    numOfPhoton_Muon_Matches++;
                  }
                  else if(closestMatch.getType() == ParticleType::electron() && lepton.getType() == ParticleType::muon())
                  {
                    numOfElectron_Muon_Matches++;
                  }
                  else if(closestMatch.getType() == ParticleType::muon() && lepton.getType() == ParticleType::electron())
                  {
                    numOfMuon_Electron_Matches++;
                  }
                  else
                  {
                    std::cout << "No match: The closest match: " << closestMatch.getType().getName() << " Lepton: " << lepton.getType().getName() << "\n";
                  }
                }
              }

          }
        }
      }
      //std::cout<< __LINE__ << " in the " << __FILE__ << std::endl;
      if(forgetIt) break;
      if(particle.getType().isQuark())
      {
        quark++;
      }
      
      if(particle.getType() == ParticleType::higgs())
      {
        if(passedDarkPhoton)
        {
          darkPhotonOrigin++;
        }
      }

      if(particle.getType() == ParticleType::photon())
      {
        numOfPhotons++;
        std::cout<<"photon\n";
      }

      if(particle.status() == 4)
      {
        proton++;
      }
    }
    
  
    for(Particle darkPhoton:darkPhotons)
    {
      if(darkPhoton.getPt()>=10 && abs(darkPhoton.getEta())<=3)
      {
        genSize++;
      }
    }
    
    //std::vector<Particle> genSimParticles(temp.begin(), temp.end());
    std::vector<LeptonJet> recoLeptonJets(lepJet->getLeptonJets());
    std::vector<Particle> lJets;

    // for (LeptonJet& lJet:recoLeptonJets)
    // {
    //   lJets.push_back(lJet);
    // }

    for (LeptonJet& lJet:recoLeptonJets)
    {
      auto leptonJetParticles = lJet.getParticles();
      lJets.push_back(lJet);
    }
    std::vector<Particle> nonlJetParticles;
    
   
    //MatchingModule::match(genSimParticles, lJets);
    MatchingModule::match(darkPhotons, lJets);
    //std::cout << "Size of matchingPairs: " << getMatchingBestPairs().getSize()<<"\n";
    lepJetSize += recoLeptonJets.size();
    darkPhoton += getMatchingBestPairs().getSize();
    std::vector<MatchingPair> matchedLeptonJets = getMatchingPairs();
    //std::cout<<getMatchingPairs().size()<<std::endl;

    // for (Particle lepton:recoParticles){
    //   lJetsCopy = std
    // }
    //leptonOrigins.push_back(std::pair<Particle,GenSimParticle>{Particle::nullParticle(),underlyingLepton.at(0)}); 
    // for (MatchingPair leptonPair : matchedLeptonJets)
    // {
    //   //std::cout<<leptonPair.first.getName()<<std::endl;
    //   if((leptonPair.first.getType() == ParticleType::darkPhoton()))
    //   {
    //     darkPhoton++;
    //   }
    //   else
    //   {
    //  
    //     for(GenSimParticle jet: leptonJet.getParticles())
    //     {
    //       // auto it = std::find_if( leptonOrigins.begin(), leptonOrigins.end(),
    //       //   [&jet](const std::pair<GenSimParticle,GenSimParticle>& element){ return element.first == jet;} );
    //       //my_file1 << particleInformationString(jet,it->second, leptonJet.getParticles())<<std::endl;
    //       my_file1 << particleInformationString(jet, leptonJet.getParticles())<<std::endl;
    //       //printGenSimParticleCollection(leptonJet, my_file1);
    //     }
    //   }
    // }
    //lepJetSize += recoLeptonJets.size();

    // MatchingPair candidate;
    Particle nullParticle(Particle::nullParticle());
    LeptonJet nullJet(nullParticle);
    //std::cout<<"The number of dark photons is: " << numOfDarkPhotons << "\n";
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
  std::cout << "Leptons From Quarks " << (double) quark / genLeptons << std::endl;
  std::cout << "Leptons From Dark Photons " << (double) darkPhotonOrigin / genLeptons << std::endl;
  std::cout << "Leptons From Squarks " << (double) squark / genLeptons << std::endl;
  std::cout << "Leptons From Protons " << (double) proton / genLeptons << std::endl;
  std::cout << "Number of Muon Matches " << numOfMuonMatches << std::endl;
  std::cout << "Number of Electron Matches " << numOfElectronMatches << std::endl;
  std::cout << "Number of No Matches " << noMatch << std::endl;
  std::cout << "Number of Photon Electron Pairings " << numOfPhoton_Electron_Matches << std::endl;
  std::cout << "Number of Photon Muon Pairings " << numOfPhoton_Muon_Matches << std::endl;
  std::cout << "Number of Muon Electron Pairings " << numOfMuon_Electron_Matches << std::endl;
  std::cout << "Number of Electron Muon Pairings " << numOfElectron_Muon_Matches << std::endl;
}

const std::vector<std::pair<Particle,LeptonJet>> LeptonJetMatchingModule::getMatchingPairs() const 
{
  auto& matchedParticles = getMatchingBestPairs().getPairs();
  std::vector<MatchingPair> matchedLeptonJets;
  for (auto& matched : matchedParticles){
    matchedLeptonJets.push_back(std::make_pair(matched.getGenParticle(),matched.getRecoParticle())); //for some reason lepton jet is assigned as recoParticle in MatchingPair
  }
  return matchedLeptonJets;
}

const bool LeptonJetMatchingModule::isQuark(GenSimParticle lepton) 
{
  if(abs(lepton.pdgId()) == 1 || abs(lepton.pdgId()) == 2 || abs(lepton.pdgId()) == 3 || abs(lepton.pdgId()) == 4)
  {
    //quark++;
    return true;
  }
  else if(abs(lepton.pdgId()) == 5 || abs(lepton.pdgId()) == 6)
  {
    //quark++;
    return true;
  }
  return false;
}
const bool LeptonJetMatchingModule::isSquark(GenSimParticle lepton)
{
  if(abs(lepton.pdgId()) == 1000001 || abs(lepton.pdgId()) == 1000002 || abs(lepton.pdgId()) == 1000003 || abs(lepton.pdgId()) == 1000004)
  {
    //squark++;
    return true;
  }
  else if(abs(lepton.pdgId()) == 1000005 || abs(lepton.pdgId()) == 1000006)
  {
    //squark++;
    return true;
  }
  return false;
}
const bool LeptonJetMatchingModule::isHiggs(GenSimParticle lepton)
{
  if(abs(lepton.pdgId()) == 25)
  {
    return true;
  }
  return false;
}

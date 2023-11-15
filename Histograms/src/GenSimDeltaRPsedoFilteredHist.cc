//INclude statments messed up during restructure

// #include "CMSAnalysis/DataCollection/interface/GenSimDeltaRPsedoFilteredHist.hh"
// #include "CMSAnalysis/Utility/interface/Particle.hh"
// #include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
// #include "DataFormats/Math/interface/deltaR.h"
// #include "CMSAnalysis/DataCollection/interface/InputModule.hh"
// #include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
// #include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"



// #include <iostream>

// using std::cout;

// GenSimDeltaRPsedoFilteredHist::GenSimDeltaRPsedoFilteredHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetReconstructionModule> lepJetModule):
//  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
//  leptonJets(lepJetModule)

// {

// }

// std::vector<double> GenSimDeltaRPsedoFilteredHist::value() const
// {
//   //call getparticles or getInput getparticles in input module level-gensim, particle type called darkphoton in particle class
//   //particles = GetInput() from input module?
//   //not in particle.cc, just pdgid == 4900022 for identifying dark photons
//   auto particles = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::darkPhoton());
//   //
//   // particles is an empty vector,
//   //std::cout<<particles[0];
//   //only getting dark photons breaks
//   //, ParticleType::darkPhoton()

//   //loop through number of events to get all final daughters
//   //finalDaughter(0...1...2...)
//   // for (auto particle : particles.getParticles())
//   // {
//   //   while (particle != particle.finalDaughter())
//   //   {
//   //     particle = particle.finalDaughter();
//   //   }
//   // }

//   // if (particle.getType() == 4900022)
//   // {
//   // }
//   std::vector<double> deltaRVector{};

//   for (GenSimParticle particle : particles.getParticles()) 
//   { 
    
//     // if (particle.getType() == Particle::Type::DarkPhoton)
//     // {
//     // }

//     if (particle.finalDaughter() != particle)
//     {
//       std::cout << "++";
//       continue;
//     }

//     std::vector<Particle> leptons;
//     //looping through lepton decays (should be only 2) and pushing to lepton list
//     //std::cout<<"ParticleType: " << particle.getName() << "\n";
//     //std::cout<<"Daughters: " << particle.numberOfDaughters() << "\n";

//     for (int j = 0; j < particle.numberOfDaughters(); ++j)
    
//     {
//       auto leptonCandidate = particle.daughter(j);
//       if (leptonCandidate.getType() == ParticleType::muon())
//       {
//         leptons.push_back(leptonCandidate);
//       }
//     }
//   std::vector<LeptonJet> leptonJetsVector = leptonJets->getLeptonJets();
//   int numberOfLeptonJets = leptonJetsVector.size();
//   bool returnBool; 
//   if(numberOfLeptonJets == 0) {
//     returnBool = false;
//   }
//   else if(numberOfLeptonJets == 1){
//     returnBool = true;

//   }
//   else {
//     returnBool = false;
//   }



//    //std::cout << leptons.size();

//     if (leptons.size() >= 2 && returnBool)
//     {
//       auto particle1FourVector = leptons[0].getFourVector();
//       auto particle2FourVector = leptons[1].getFourVector();
//       double deltaR = reco::deltaR(particle1FourVector, particle2FourVector);
//       deltaRVector.push_back(deltaR);
//     }
//   }
//   return deltaRVector;
// }
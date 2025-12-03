#include "CMSAnalysis/Utility/interface/Utility.hh"
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <numeric>


std::string Utility::substitute(std::string original, std::string target, std::string subs)
{
  size_t start_pos = 0;
  while((start_pos = original.find(target, start_pos)) != std::string::npos) {
      original.replace(start_pos, target.length(), subs);
      start_pos += subs.length(); 
  }
  return original;
}

std::string Utility::getFullPath(std::string fileName)
{

  return (getBasePath()+"DataCollection/bin/textfiles/" + fileName);
}

std::string Utility::getBasePath()
{
  std::string location = std::getenv("CMSSW_BASE"); 
  return (location + "/src/CMSAnalysis/");
}


std::string Utility::removeSpaces(std::string input) 
{
  std::string outputString;
  for(int i = 0; i < (int) input.length(); i++) {
	  if(input[i] != ' ') {
		  outputString = outputString + input[i];
	  }
	}
  return outputString;
}

std::vector<std::string> Utility::channelMaker(std::vector<std::string> particles, int length, bool sameSign) {
  std::vector<std::string> toAdd;
  std::vector<std::string> channels;
  for(std::string particle : particles) {
    for(int j = 0; j < pow(particles.size(), (length - 1)); j++) {
      toAdd.push_back(particle);
    }
  }
  for(int k = 1; k < length; k++) {
    for(int m = 0; m < pow(particles.size(), length); m++) {
      toAdd.at(m) = toAdd.at(m) + particles.at(((int) (m / (pow(particles.size(), (length - k - 1)))) % (int) particles.size()));
    }
  }
  if(sameSign) {
    for(std::string channel : toAdd) {
      bool keepLooping = true;
      bool keepChannel = true;
      std::vector<std::string> pairs1;
      std::vector<std::string> pairs2;
      int count2 = 0;
      if(toAdd.at(0) == channel) {
        keepLooping = false;
      }
      while (keepLooping == true) {
        std::string toAddStorage = toAdd.at(count2);
        std::string channelStorage = channel;
        std::sort(toAddStorage.begin(), toAddStorage.end());
        std::sort(channelStorage.begin(), channelStorage.end());
        if(toAddStorage == channelStorage) {
          for(int l = 0; l < ((int) toAdd.at(count2).length() - 1); l += 2) {
            std::string pair1 = toAdd.at(count2).substr(l, 2);
            std::string pair2 = channel.substr(l, 2);
            pairs1.push_back(pair1);
            pairs2.push_back(pair2);
          }
          for(int n = 0; n < (int) pairs1.size(); n++) {
            std::sort(pairs1.at(n).begin(), pairs1.at(n).end());
            std::sort(pairs2.at(n).begin(), pairs2.at(n).end());
          }
          std::sort(pairs1.begin(), pairs1.end(), comparator);
          std::sort(pairs2.begin(), pairs2.end(), comparator);
          bool sameVec = true;
          for(int o = 0; o < (int) pairs1.size(); o++) {
            if(pairs1.at(o) != pairs2.at(o)) {
              sameVec = false;
            }
          }
          if (sameVec){
            keepChannel = false;
            keepLooping = false;
          }
          pairs1.clear();
          pairs2.clear();
        }
        if(toAdd.at(count2 + 1) == channel) {
          keepLooping = false;
        }
        count2++;
      }
      if(keepChannel) {
        channels.push_back(channel);
      }
    }
  }
  else {
    for(std::string channel : toAdd) {
      bool keepLooping = true;
      bool keepChannel = true;
      int count2 = 0;
      if(toAdd.at(0) == channel) {
        keepLooping = false;
      }
      while (keepLooping == true) {
        std::string toAddStorage = toAdd.at(count2);
        std::string channelStorage = channel;
        std::sort(toAddStorage.begin(), toAddStorage.end());
        std::sort(channelStorage.begin(), channelStorage.end());
        if(toAddStorage == channelStorage) {
          keepChannel = false;
        }
        if(toAdd.at(count2 + 1) == channel) {
          keepLooping = false;
        }
        count2++;
      }
      if(keepChannel) {
        channels.push_back(channel);
       }
    }
  }
  channels.push_back("none");
  return channels;
}

int Utility::gcf(std::vector<int> nums) {
  int toReturn = 0;
  int previous = nums[0];

  for(int i = 1; static_cast<std::vector<int>::size_type>(i) < nums.size(); i++) {
    toReturn = std::gcd(previous, nums[i]);
    previous = toReturn;
  }

  return toReturn;
}

std::string Utility::identifyChannel(ParticleCollection<Particle> particles)
{
  std::vector<std::pair<char, char>> leptons;
  std::string positiveLeptons;
  std::string negativeLeptons;
  int nTaus = 0;

  for (const auto& particle : particles)
  {
    char flavor;

    if (particle.getType() == ParticleType::electron())
    {
      flavor = 'a';
    }
    else if (particle.getType() == ParticleType::muon())
    {
      flavor = 'b';
    }
    else if (particle.getType() == ParticleType::tau()) //taus don't undergo the charge sorting process
    {
      ++nTaus; 
      continue;
    }
    else
    {
      continue;
    }
   
    if (particle.getCharge() > 0)
    {
      positiveLeptons += flavor;
    }
    else
    {
      negativeLeptons += flavor;
    }
  }

//std::cout << "Ntaus: " << nTaus << "  pos: " << positiveLeptons << "  neg: " << negativeLeptons << "                 " << '\n' ;

// Fill 'c' in where there is a space
// up to the number of taus --> make a while loop that runs out of Taus, for each one add a c
// add it to the string that has the fewest leptons (.length stuff)
// if they are the same length, add it to the one that has more electrons, and if it's same, add to one with more muons
//make sure there's automatic underscores for Taus
// we're looking at just positive or just negative pairs - you have to add a c no matter what (turns into tau)
  
  while (nTaus > 0) //this while loop is responsible for sorting taus into the category with less leptons
  {
    // decide where to put the tau
    if (positiveLeptons.size() < negativeLeptons.size())
    {
      positiveLeptons += 'c';
    }
    else if (negativeLeptons.size() < positiveLeptons.size())
    {
      negativeLeptons += 'c';
    }
    else
    {
      // tie in length, break with electron count
      int posElectrons = std::count(positiveLeptons.begin(), positiveLeptons.end(), 'a');
      int negElectrons = std::count(negativeLeptons.begin(), negativeLeptons.end(), 'a');

      if (posElectrons > negElectrons)
      {
        positiveLeptons += 'c';
      }
      else if (negElectrons > posElectrons)
      {
        negativeLeptons += 'c';
      }
      else
      {
        // tie again, break with muon count
        int posMuons = std::count(positiveLeptons.begin(), positiveLeptons.end(), 'b');
        int negMuons = std::count(negativeLeptons.begin(), negativeLeptons.end(), 'b');

        if (posMuons > negMuons) // assigns 
        {
          positiveLeptons += 'c';
        }
        else
        {
          negativeLeptons += 'c';
        }
      }
    }

    --nTaus;
  }

  // pad with 'x' up to 2 leptons each
  while (positiveLeptons.size() < 2)
  {
    positiveLeptons += 'x'; //makes sure there are two leptons each --> if not, pads with x which becomes underscore
  }
  while (negativeLeptons.size() < 2) 
  {
    negativeLeptons += 'x';
  }

  if (positiveLeptons.size() > 2)
    positiveLeptons = positiveLeptons.substr(0, 2);

  if (negativeLeptons.size() > 2)
    negativeLeptons = negativeLeptons.substr(0, 2);

  // sorts by flavor
  std::sort(positiveLeptons.begin(), positiveLeptons.end());
  std::sort(negativeLeptons.begin(), negativeLeptons.end());

  std::string first, second;

  if (positiveLeptons < negativeLeptons)
  {
    first = positiveLeptons;
    second = negativeLeptons;
  }
  else
  {
    first = negativeLeptons;
    second = positiveLeptons;
  }

  // builds signature with substitutions
  std::string signature = first + second;
 
  signature = substitute(signature, "a", "e");
  signature = substitute(signature, "b", "u");
  signature = substitute(signature, "c", "t");
  signature = substitute(signature, "x", "_");

  //std::cout << "Signature: " << signature << " (length = " << signature.length() << ")" 
  //<< " Positives: " << positiveLeptons << " Negatives: " << negativeLeptons << std::endl;
  // if (signature.size() > 4)
  //   {
  //     std::cout << "Positives: " << positiveLeptons << " Negatives: " << negativeLeptons << " Signature: " << signature << std::endl;
  //   }
     //have something that looks at signature.length. if signature.length is greater than 4, print it out.
    return signature;
}


void writeRootObj(TFile *outFile, const std::string &path, TObject *obj)
{
  if (outFile->Get(path.c_str()))
    {
        if (outFile->cd(path.c_str()) != kTRUE)
        {
            throw std::runtime_error("Failed to change directory to " + path);
        }
    }
    else
    {
      if (outFile->mkdir(path.c_str(), "", true) == nullptr)
        {
            throw std::runtime_error("Failed to create directory " + path);
        }
      if (outFile->cd(path.c_str()) != kTRUE)
        {
            throw std::runtime_error("Failed to change directory to " + path);
        }     
    }

    if (obj->Write() == 0)
    {
        throw std::runtime_error("Failed to write object " + std::string(obj->GetName()) + " to " + path);
    };

    outFile->cd();
}

std::pair<std::pair<Particle, Particle>, std::pair<Particle, Particle>> Utility::findBestLeptonPairing(ParticleCollection<Particle> leptons)
{

  // Collection Initialize
  ParticleCollection positiveLeptons;
  ParticleCollection negativeLeptons;
  ParticleCollection taus;

  // Sorts Leptons by charge
  for (auto lepton : leptons) {
    if (lepton.getType() == ParticleType::tau()) {
      taus.addParticle(lepton);
    }
    else if (lepton.getCharge() == 1) {
      positiveLeptons.addParticle(lepton);
    }
    else if (lepton.getCharge() == -1) {
      negativeLeptons.addParticle(lepton);
    }
  }

  // Handle based on number of taus
  int tauCount = taus.size();
  
  // No taus - use the original logic for charged leptons
  if (tauCount == 0) 
  {
    
    //double positivePairInvariantMass = Particle::invariantMass(positiveLeptons[0], positiveLeptons[1]);
    //double negativePairInvariantMass = Particle::invariantMass(negativeLeptons[0], negativeLeptons[1]);
    
    int posElectronCount = 0;
    int negElectronCount = 0;

    if (positiveLeptons[0].getType() == ParticleType::electron()) ++posElectronCount;
    if (positiveLeptons[1].getType() == ParticleType::electron()) ++posElectronCount;
    if (negativeLeptons[0].getType() == ParticleType::electron()) ++negElectronCount;
    if (negativeLeptons[1].getType() == ParticleType::electron()) ++negElectronCount;

    if (posElectronCount == negElectronCount)
      return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    
    if (posElectronCount > negElectronCount) {
      return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    } 
    else {
      return {{negativeLeptons[0], negativeLeptons[1]}, {positiveLeptons[0], positiveLeptons[1]}};
    }
  }
  
  // 1 tau - pair with the lepton of different charge than the majority
  else if (tauCount == 1) {
    // Remaining leptons
    std::vector<Particle> otherLeptons;
    for (auto& lepton : leptons) {
      if (lepton.getType() != ParticleType::tau()) {
        otherLeptons.push_back(lepton);
      }
    }

    
    // Count charges of other leptons
    int positiveCount = 0, negativeCount = 0;
    for (auto& lepton : otherLeptons) {
      if (lepton.getCharge() == 1) positiveCount++;
      else if (lepton.getCharge() == -1) negativeCount++;
    }
    
    // Pair tau with the minority charge lepton
    Particle tauPartner = leptons[0]; // Placeholder
    ParticleCollection firstPair;
    ParticleCollection secondPair;
    
    if (positiveCount > negativeCount) {
      // Find the negative lepton to pair with tau
      for (auto& lepton : otherLeptons) {
        if (lepton.getCharge() == -1) {
          tauPartner = lepton;
          break;
        }
      }
      
      // Add tau and partner to first pair
      firstPair.addParticle(taus[0]);
      firstPair.addParticle(tauPartner);
      
      // Add remaining leptons to second pair
      for (auto& lepton : otherLeptons) {
        if (!(lepton == tauPartner)) {
          secondPair.addParticle(lepton);
        }
      }
    }
    else {
      // Find the positive lepton to pair with tau
      for (auto& lepton : otherLeptons) {
        if (lepton.getCharge() == 1) {
          tauPartner = lepton;
          break;
        }
      }
      
      // Add tau and partner to first pair
      firstPair.addParticle(taus[0]);
      firstPair.addParticle(tauPartner);
      
      // Add remaining leptons to second pair
      for (auto& lepton : otherLeptons) {
        if (!(lepton == tauPartner)) {
          secondPair.addParticle(lepton);
        }
      }
    }
    
    //double firstInvariantMass = Particle::invariantMass(firstPair[0], firstPair[1]);
    //double secondInvariantMass = Particle::invariantMass(secondPair[0], secondPair[1]);

    return {{firstPair[0], firstPair[1]}, {secondPair[0], secondPair[1]}};
  }
  
  // 2 taus - balance charges if possible, otherwise maximize invariant mass
  else if (tauCount == 2) {
    // Remaining leptons
    std::vector<Particle> otherLeptons;
    for (auto& lepton : leptons) {
      if (lepton.getType() != ParticleType::tau()) {
        otherLeptons.push_back(lepton);
      }
    }
   
    // Check if other leptons have the same sign
    bool sameSign = (otherLeptons[0].getCharge() == otherLeptons[1].getCharge());
    
    if (sameSign) {
      // If other leptons have same sign, pair the taus together
      //double tausPairMass = Particle::invariantMass(taus[0], taus[1]);
      //double otherPairMass = Particle::invariantMass(otherLeptons[0], otherLeptons[1]);

      return {{taus[0], taus[1]}, {otherLeptons[0], otherLeptons[1]}};
    }
    else {
      // If other leptons have opposite sign, try all combinations and pick highest invariant mass
      double mass1 = Particle::invariantMass(taus[0], otherLeptons[0]) + 
                    Particle::invariantMass(taus[1], otherLeptons[1]);
                    
      double mass2 = Particle::invariantMass(taus[0], otherLeptons[1]) + 
                    Particle::invariantMass(taus[1], otherLeptons[0]);
      
      if (mass1 >= mass2) {
        //double pair1Mass = Particle::invariantMass(taus[0], otherLeptons[0]);
        //double pair2Mass = Particle::invariantMass(taus[1], otherLeptons[1]);
        return {{taus[0], otherLeptons[0]}, {taus[1], otherLeptons[1]}};
      }
      else {
        //double pair1Mass = Particle::invariantMass(taus[0], otherLeptons[1]);
        //double pair2Mass = Particle::invariantMass(taus[1], otherLeptons[0]);
        return {{taus[0], otherLeptons[1]}, {taus[1], otherLeptons[0]}};
      }
    }
  }
        // 3 taus - try all combinations and pick the one with highest invariant mass sum
      else if (tauCount == 3) {
        // Get the one non-tau lepton
        Particle otherLepton = leptons[0]; // Placeholder
        for (auto& lepton : leptons) {
          if (lepton.getType() != ParticleType::tau()) {
            otherLepton = lepton;
            break;
          }
        }
        
        // Try all possible combinations and pick the one with highest invariant mass
        double bestMassSum = 0.0;
        std::pair<std::pair<Particle, Particle>, std::pair<Particle, Particle>> bestParticles = {{leptons[0], leptons[0]}, {leptons[0], leptons[0]}}; // Placeholder

        // Try pairing each tau with the other lepton
        for (int i = 0; i < 3; i++) {
          ParticleCollection firstPair;
          ParticleCollection secondPair;
          
          // First pair: tau[i] + otherLepton
          firstPair.addParticle(taus[i]);
          firstPair.addParticle(otherLepton);
          
          // Second pair: remaining two taus
          for (int j = 0; j < 3; j++) {
            if (j != i) {
              secondPair.addParticle(taus[j]);
            }
          }
          
          double mass1 = Particle::invariantMass(firstPair[0], firstPair[1]);
          double mass2 = Particle::invariantMass(secondPair[0], secondPair[1]);
          double massSum = mass1 + mass2;
          
          if (massSum > bestMassSum) {
            bestMassSum = massSum;
            bestParticles = {{firstPair[0], firstPair[1]}, {secondPair[0], secondPair[1]}};
          }
        }
        
        return {bestParticles};
      }
      
      // 4 taus - try all combinations and pick the one with highest invariant mass sum
      else if (tauCount == 4) {
        // Try all possible pairings of the 4 taus (3 unique ways to pair them)
        //double bestMassSum = 0.0;
        std::pair<double, double> bestMasses;
        
        // Pairing option 1: (0,1) and (2,3)
        double mass1_1 = Particle::invariantMass(taus[0], taus[1]);
        double mass1_2 = Particle::invariantMass(taus[2], taus[3]);
        double sum1 = mass1_1 + mass1_2;
        
        // Pairing option 2: (0,2) and (1,3)
        double mass2_1 = Particle::invariantMass(taus[0], taus[2]);
        double mass2_2 = Particle::invariantMass(taus[1], taus[3]);
        double sum2 = mass2_1 + mass2_2;
        
        // Pairing option 3: (0,3) and (1,2)
        double mass3_1 = Particle::invariantMass(taus[0], taus[3]);
        double mass3_2 = Particle::invariantMass(taus[1], taus[2]);
        double sum3 = mass3_1 + mass3_2;
        
        // Find the best pairing (highest invariant mass sum)
        if (sum1 >= sum2 && sum1 >= sum3) {
          return {{taus[0], taus[1]}, {taus[2], taus[3]}};
        } 
        else if (sum2 >= sum1 && sum2 >= sum3) {
          return {{taus[0], taus[2]}, {taus[1], taus[3]}};
        } 
        else {
          return {{taus[0], taus[3]}, {taus[1], taus[2]}};
        }
      }
      else{
        throw std::runtime_error("Unexpected number of taus in findBestLeptonPairing.");
      }
  }

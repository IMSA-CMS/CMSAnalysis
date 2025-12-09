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

//   double invariantMass(Particle p1, Particle p2, double met_x, double met_y, bool caseTwo)
// {
//     struct Vector2D {
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

//     // this is good to use
//     // Solve for α1 and α2
//     // Matrix:
//     // [ p1x  p2x ] [a1] = [ METx ]
//     // [ p1y  p2y ] [a2]   [ METy ]
    
//     double det = p1T.x * p2T.y - p1T.y * p2T.x;

//     std::cout << "det = " << det << "\n";

//     if (std::abs(det) < 1e-9) {
//         std::cout << "DET=0 — method fails.\n";
//         return -1;
//     }

//     double alpha1 = ( MET.x * p2T.y - MET.y * p2T.x ) / det;
//     double alpha2 = ( p1T.x * MET.y - p1T.y * MET.x ) / det;

//     std::cout << "alpha1 = " << alpha1 << "\n";
//     std::cout << "alpha2 = " << alpha2 << "\n";

//     if (alpha1 < 0 || alpha2 < 0) {
//         std::cout << "Negative energy scaling — return -1.\n";
//         return -1;
//     }
   
//     double E1 = p1.energy();
//     double E2 = p2.energy();

//     double pz1 = p1.pz();
//     double pz2 = p2.pz();

//     double m_vis =
//         std::sqrt( std::pow(E1 + E2, 2)
//         - std::pow(p1.px()+p2.px(), 2)
//         - std::pow(p1.py()+p2.py(), 2)
//         - std::pow(p1.pz()+p2.pz(), 2) );
 
//     double m_vis1 = std::sqrt(E1*E1 - (p1.px()*p1.px() + p1.py()*p1.py() + pz1*pz1));
//     double m_vis2 = std::sqrt(E2*E2 - (p2.px()*p2.px() + p2.py()*p2.py() + pz2*pz2));

//     std::cout << "m_vis  = " << m_vis << "\n";
//     std::cout << "m_vis1 = " << m_vis1 << "\n";
//     std::cout << "m_vis2 = " << m_vis2 << "\n";
  
//     if (!caseTwo) {
//         double result = m_vis * std::sqrt((1 + alpha1) * (1 + alpha2));
//         std::cout << "CASE 1 MASS = " << result << "\n";
//         return result;
//     }

//     double m2 =
//           (1+alpha1)*(1+alpha1)*m_vis1*m_vis1
//         + (1+alpha2)*(1+alpha2)*m_vis2*m_vis2;

//     if (m2 < 0) {
//         std::cout << "m² < 0 — return 0.\n";
//         return 0;
//     }

//     double result = std::sqrt(m2);
//     std::cout << "CASE 2 MASS = " << result << "\n";
//     return result;
// }
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

void writeRootObj(TFile *outFile, const std::string &path, TObject *obj)
{
    if (!outFile->cd(path.c_str()))
    {
        outFile->mkdir(path.c_str(), "", true);
        outFile->cd(path.c_str());
    }

    obj->Write();

    outFile->cd();
}

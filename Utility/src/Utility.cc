#include "CMSAnalysis/Utility/interface/Utility.hh"
#include <algorithm>
#include <boost/algorithm/string/erase.hpp>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>

std::string Utility::substitute(std::string original, std::string target, std::string subs)
{
    size_t start_pos = 0;
    while ((start_pos = original.find(target, start_pos)) != std::string::npos)
    {
        original.replace(start_pos, target.length(), subs);
        start_pos += subs.length();
    }
    return original;
}

std::string Utility::getFullPath(std::string fileName)
{
    return (getBasePath() + "DataCollection/bin/textfiles/" + fileName);
}

std::string Utility::getBasePath()
{
    std::string location = std::getenv("CMSSW_BASE");
    return location + "/src/CMSAnalysis/";
}

std::string Utility::removeSpaces(std::string input)
{
    return boost::algorithm::erase_all_copy(input, " ");
}

std::vector<std::string> Utility::channelMaker(std::vector<std::string> particles, int length, bool sameSign)
{
    std::vector<std::string> toAdd;
    for (const std::string &particle : particles)
    {
        for (int j = 0; j < pow(particles.size(), (length - 1)); j++)
        {
            toAdd.push_back(particle);
        }
    }
    for (int k = 1; k < length; k++)
    {
        for (int m = 0; m < pow(particles.size(), length); m++)
        {
            toAdd.at(m) = toAdd.at(m) +
                          particles.at(((int)(m / (pow(particles.size(), (length - k - 1)))) % (int)particles.size()));
        }
    }
    std::vector<std::string> channels;
    for (const std::string &channel : toAdd)
    {
        if (toAdd.at(0) == channel)
        {
            continue;
        }

        std::string channelStorage = channel;
        std::ranges::sort(channelStorage);
        for (const auto &a : toAdd)
        {
            if (a == channel)
            {
                channels.push_back(channel);
                break;
            }

            std::string toAddStorage = a;
            std::ranges::sort(toAddStorage);

            if ((toAddStorage != channelStorage) || (!sameSign))
            {
                break;
            }

            std::vector<std::string> pairs1;
            std::vector<std::string> pairs2;

            for (int l = 0; l < ((int)a.length() - 1); l += 2)
            {
                std::string pair1 = a.substr(l, 2);
                std::string pair2 = channel.substr(l, 2);
                std::ranges::sort(pair1);
                std::ranges::sort(pair2);
                pairs1.push_back(pair1);
                pairs2.push_back(pair2);
            }
            std::ranges::sort(pairs1);
            std::ranges::sort(pairs2);
            if (pairs1 == pairs2)
            {
                break;
            }
        }
    }
    channels.push_back("none");
    return channels;
}

int Utility::gcf(std::vector<int> nums)
{
    int toReturn = 0;
    int previous = nums.at(0);

    for (auto num : nums)
    {
        toReturn = std::gcd(previous, num);
        previous = toReturn;
    }

    return toReturn;
}

std::string Utility::identifyChannel(ParticleCollection<Particle> particles)
{
    std::string positiveLeptons;
    std::string negativeLeptons;

    for (const auto &particle : particles)
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
        else if (particle.getType() == ParticleType::tau())
        {
            flavor = 'c';
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

    if (positiveLeptons.size() > 2)
    {
        positiveLeptons = positiveLeptons.substr(0, 2);
    }

    if (negativeLeptons.size() > 2)
    {
        negativeLeptons = negativeLeptons.substr(0, 2);
    }

    // sorts by flavor
    std::ranges::sort(positiveLeptons);
    std::ranges::sort(negativeLeptons);

    std::string first;
    std::string second;

    if (positiveLeptons.length() > negativeLeptons.length())
    {
        first = positiveLeptons;
        second = negativeLeptons;
    }
    else if (positiveLeptons.length() < negativeLeptons.length())
    {
        first = negativeLeptons;
        second = positiveLeptons;
    }
    else if (positiveLeptons < negativeLeptons)
    {
        first = positiveLeptons;
        second = negativeLeptons;
    }
    else
    {
        first = negativeLeptons;
        second = positiveLeptons;
    }

    // pad with '_' up to 2 leptons each
    while (first.size() < 2)
    {
        first += '_';
    }
    while (second.size() < 2)
    {
        second += '_';
    }

    // builds signature with substitutions
    std::string signature = first + second;

    signature = substitute(signature, "a", "e");
    signature = substitute(signature, "b", "u");
    signature = substitute(signature, "c", "t");

    // std::cout << "Signature: " << signature << " (length = " << signature.length() << ")"
    //<< " Positives: " << positiveLeptons << " Negatives: " << negativeLeptons << std::endl;
    //  if (signature.size() > 4)
    //    {
    //      std::cout << "Positives: " << positiveLeptons << " Negatives: " << negativeLeptons << " Signature: " <<
    //      signature << std::endl;
    //    }
    // have something that looks at signature.length. if signature.length is greater than 4, print it out.
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

std::pair<std::pair<Particle, Particle>, std::pair<Particle, Particle>> Utility::findBestLeptonPairing(
    ParticleCollection<Particle> leptons)
{
    // Collection Initialize
    ParticleCollection positiveLeptons;
    ParticleCollection negativeLeptons;
    int posElectronCount = 0;
    int negElectronCount = 0;
    int posMuonCount = 0;
    int negMuonCount = 0;

    // Sorts Leptons by charge
    for (const auto &lepton : leptons)
    {
        if (lepton.getCharge() == 1)
        {
            positiveLeptons.addParticle(lepton);
            if (lepton.getType() == ParticleType::electron())
            {
                posElectronCount += 1;
            }
            if (lepton.getType() == ParticleType::muon())
            {
                posMuonCount += 1;
            }
        }
        else if (lepton.getCharge() == -1)
        {
            negativeLeptons.addParticle(lepton);
            if (lepton.getType() == ParticleType::electron())
            {
                negElectronCount += 1;
            }
            if (lepton.getType() == ParticleType::muon())
            {
                negMuonCount += 1;
            }
        }
    }

    if (posElectronCount > negElectronCount)
    {
        return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    }
    else if (posElectronCount < negElectronCount)
    {
        return {{negativeLeptons[0], negativeLeptons[1]}, {positiveLeptons[0], positiveLeptons[1]}};
    }

    if (posMuonCount >= negMuonCount)
    {
        return {{positiveLeptons[0], positiveLeptons[1]}, {negativeLeptons[0], negativeLeptons[1]}};
    }
    else
    {
        return {{negativeLeptons[0], negativeLeptons[1]}, {positiveLeptons[0], positiveLeptons[1]}};
    }
}

#ifndef LOAD_PARTICLE_DATA_SCRIPT_HH
#define LOAD_PARTICLE_DATA_SCRIPT_HH

#include "ParticleType.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

const ParticleType* loadParticle(std::ifstream& file)
{
    char tempString[100];
    int pdgID;
    std::string name;
    int chargeType;

    file.ignore(100, '\"');
    file.getline(tempString, 100, '\"');

    pdgID = std::stoi(std::string(tempString));

    file.ignore(100, '\"');
    file.getline(tempString, 100, '\"');

    name = std::string(tempString);

    //Skip to chargeType
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');
    file.ignore(100, '\"');

    file.getline(tempString, 100, '\"');
    chargeType = std::stoi(std::string(tempString));

    //Blank vector temporarily
    std::vector<SingleParticleHist> typeParticleHists;
    std::vector<CollectionHist> typeCollectionHists;
    
    return &ParticleType::registerType(name, pdgID, chargeType, typeParticleHists, typeCollectionHists);
}


std::unordered_map<int, const ParticleType*> getParticleDatabase(const std::string& fileName)
{
    std::ifstream file;
    std::unordered_map<int, const ParticleType*> map;

    if (!file.is_open())
    {
        file.open(fileName);
        // std::cout << "File opened successfully\n";
        // char stringOne[100];
        // file.getline(stringOne, 100, '\n');
        // std::cout << stringOne << "\n";
    }
    else
    {
        std::cout << fileName << " is already open!\n";
        return map;
    }

    for (int i = 0; true; ++i)
    {
        //std::cout << "Iteration number " << i << "\n";
        char peekedChar = file.peek();

        //std::cout << "Peeked char = " << peekedChar << "\n";

        if (peekedChar == 'p')
        {
            //Adds particle to map
            auto particleType = loadParticle(file);
            map.insert(std::pair<int, const ParticleType*>(particleType->getpdgId(), particleType));
            //std::cout << "Particle key " << i << "\n";
        }
        else if (peekedChar == EOF)
        {
            //std::cout << "End of file reached\n";
            break;
        }
        file.ignore(100, '\n');
        
    }

    file.close();
    return map;
}

void testLoadParticleScript()
{
    auto map = getParticleDatabase("ParticleData.txt");
    if (map.find(1) != map.end())
    {
        std::cout << "Particle found\n";
    }
    else
    {
        std::cout << "Particle not found\n";
    }
    std::cout << "Particle Name: " << map[1]->getName() << "\n";
    std::cout << "Particle pdgID: " << map[1]->getpdgId() << "\n";
    std::cout << "Particle Charge: " << map[1]->getCharge() << "\n";
}

#endif
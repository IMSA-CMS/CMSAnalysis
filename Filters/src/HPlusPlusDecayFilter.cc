#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"

HPlusPlusDecayFilter::HPlusPlusDecayFilter(EventInput::RecoLevel isGenSim) :
    typeGenSim(isGenSim)
{}

int HPlusPlusDecayFilter::getIndex(std::string* arr, std::string elem, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].compare(elem) == 0)
        {
            return i;
        }
    }
    return -1;
}

std::string HPlusPlusDecayFilter::getFilterString(const EventInput* inputMod) const
{
    return getState(inputMod); 
}

std::string HPlusPlusDecayFilter::getState(const EventInput* inputMod) const
{
    auto genSim = inputMod->getParticles(EventInput::RecoLevel::GenSim);
    auto reco = inputMod->getLeptons(EventInput::RecoLevel::Reco);

    if (typeGenSim == EventInput::RecoLevel::GenSim)
    {
        std::string hPlus = "";
        std::string hMinus = "";

        std::string c1str[] = {"e", "u", "t"};
        std::string c2str[] = {"ee", "eu", "et", "uu", "ut", "tt"};
        ParticleCollection<Particle> collection;
        for (const auto& particle : genSim) // Look for H++ and H-- decays
        {
            GenSimParticle genSimParticle = GenSimParticle(particle);

            if ((genSimParticle.pdgId() == 9900041 || genSimParticle.pdgId() == 9900042) &&
                genSimParticle == genSimParticle.finalDaughter() &&
                genSimParticle.numberOfDaughters() == 2) // H++
            {
                collection.addParticle(genSimParticle.daughter(0));
                collection.addParticle(genSimParticle.daughter(1));
                
            }
            else if ((genSimParticle.pdgId() == -9900041 || genSimParticle.pdgId() == -9900042) &&
                     genSimParticle == genSimParticle.finalDaughter() &&
                     genSimParticle.numberOfDaughters() == 2) // H--
            {
      
                collection.addParticle(genSimParticle.daughter(0));
                collection.addParticle(genSimParticle.daughter(1));
                
            }

            if (collection.size() == 4)
            {
                break;
            }
        }
        std::string channel = "GenSim " + Utility::identifyChannel(collection);

        return channel;
    }

    else if (typeGenSim == EventInput::RecoLevel::Reco) 
    {
        // identifies channel using Utility function 
        std::string channel = Utility::identifyChannel(reco);
        //std::cout << channel << '\n';
        return "Reco " + channel;
    }

    return "none"; 
}




   

#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"
#include "CMSAnalysis/Modules/interface/MultiChannelTreeMakerModule.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "TTree.h"

#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

using std::begin;
using std::end;


MultiChannelTreeMakerModule::MultiChannelTreeMakerModule(std::shared_ptr<SpecialVariableModule> iSpecialVariable, std::string iName) :
    TreeMakerModule(iSpecialVariable, iName)
{
}

//the main process function called for each event
bool MultiChannelTreeMakerModule::process()
{
    auto recoParticles = getParticles();
    //std::string filter = Utility::identifyChannel(recoParticles[0]);
    std::string recoFilter = Utility::identifyChannel(recoParticles[0]);
    //std::string genSimFilter = Utility::identifyChannel(recoParticles[1]);
    std::string filter = recoFilter;

    if(trees.find(filter) == trees.end())
    {
        std::string treeName = filter + getTreeName();
        TTree* channelTree = new TTree(treeName.c_str(), treeName.c_str());
        getSpecialVariable()->addVariablesToTree(channelTree); 
        trees.insert({filter, channelTree});
    }
    if (trees.find(filter) != trees.end())
    {
        for (const auto& collection : recoParticles)
        {
            for (const auto& particle : collection.getParticles())
            {
                ParticleCollection<Particle> singleParticleCollection(std::vector<Particle>{particle});
                this->getSpecialVariable()->calculateVariables(singleParticleCollection);
            }
        }
        trees[filter]->Fill();
    }
    return true;  
}


// initialize all trees corresponding to different event channels
// void MultiChannelTreeMakerModule::initialize()
// {
//     std::vector<std::string> channels = {
//         // four lepton channels with none missing
//         "eeee_", "eeeu_", "eeuu_", "euuu_", "uuuu_",
//         "eeet_", "eeut_", "eett_", "euet_", "euut_", "eutt_",
//         "uuet_", "etut_", "uuut_", "uutt_", "etet_", "ettt_", "utut_", "uttt_", "tttt_",

//         // three lepton channels, one missing
//         "eeu_", "eue_", "euu_", "uue_", "uuu_",
//         "eet_", "eut_", "uut_", "ete_", "etu_", "ett_",
//         "ute_", "utu_", "utt_", "ttu_", "ttt_",

//         // two lepton channels, two missing
//         "eu__", "e_u_", "uu__", "u_u_",
//         "et__", "e_t_", "u_t_", "tt__", "t_t_",
//     };

//     // For each channel name, create a TTree and add variables to it
//     for (const auto& channel : channels)
//     {
//         std::string treeName = channel + getTreeName();
//         TTree* channelTree = new TTree(treeName.c_str(), treeName.c_str());
//         getSpecialVariable()->addVariablesToTree(channelTree); 
//         trees.insert({channel, channelTree});
//     }

// }

// write all trees to the output file at the end of processing
void MultiChannelTreeMakerModule::finalize()
{
    for (auto& tree : trees)
    {
        tree.second->Write();
    }
}

// fill all trees with the current event data (usually called if multiple trees filled per event)
void MultiChannelTreeMakerModule::fillTree()
{
    for (auto& tree : trees)
    {
        tree.second->Fill();
    }
}

std::vector<ParticleCollection<Particle>> MultiChannelTreeMakerModule::getParticles() const
{
    return {getInput()->getParticles(EventInput::RecoLevel::Reco)};
}




// classify the event into a channel signature string based on generated-level leptons
// std::string MultiChannelTreeMakerModule::classifyChannel() const
// {
//     auto genParticlesBase = getInput()->getParticles(EventInput::RecoLevel::GenSim);
    
//     std::vector<std::string> plusLeptons;  
//     std::vector<std::string> minusLeptons; 
//     #include "CMSAnalysis/Utility/interface/GenSimParticle.hh"  // included for Particle class

//     // loop over all gen-level particles, differentiate between leptonic/hadronic decays 
//     for (const auto& baseParticle : genParticlesBase)
//     {
//         const auto& genParticle = static_cast<const Particle&>(baseParticle);

//         int pdgID = genParticle.getID();  
//         int absID = abs(pdgID);
//         std::string leptonType;

//         // identify particle flavor by PDG ID absolute value (e, u and t)
//         if (genParticle.getType()==ParticleType::electron())leptonType = "e";    
//         else if (absID == 13) leptonTyp = "u"; //change to same format as 
//         else if (absID == 15)                   
//         //  {
//         //     // check if tau decayed hadronically (no leptonic daughters)
//         //     bool isHadronic = true;
//         //     for (const auto& daughter : genParticle.getDaughters())
//         //     {
//         //         int dauID = abs(daughter.getID());
//         //         // if daughter is electron or muon, tau decay is leptonic, skip this particle
//         //         if (dauID == 11 || dauID == 13)
//         //         {
//         //             isHadronic = false;
//         //             break;
//         //         }
//         //     }
//         //     if (!isHadronic) continue;  // skip leptonic taus
//         //     leptonType = "t";           // hadronic tau
//         // } 
//         else continue;  // skip non-lepton particles
//         leptonType += (pdgID > 0) ? "+" : "-";

//         // store leptons in separate vectors based on charge
//         if (pdgID > 0)
//             plusLeptons.push_back(leptonType);
//         else
//             minusLeptons.push_back(leptonType);
//     }

//     // sort leptons by charge group and alphabetically by flavor
//     std::sort(plusLeptons.begin(), plusLeptons.end());
//     std::sort(minusLeptons.begin(), minusLeptons.end());

//     // concatenate positive and negative leptons into a single vector
//     std::vector<std::string> allLeptons = plusLeptons;
//     allLeptons.insert(allLeptons.end(), minusLeptons.begin(), minusLeptons.end());

//     // build signature string using just the flavor letter (e, u, t) without charge
//     std::string signature;
//     for (const auto& lep : allLeptons)
//         signature += lep[0];

//     // pad with underscores to ensure the signature has length 4
//     while (signature.size() < 4)
//         signature += '_';

//     // add trailing underscore to follow naming convention (e.g. "euut_")
//     signature += "_";

//     // check if the signature corresponds to a known tree channel
//     if (trees.find(signature) != trees.end())
//         return signature;

//     // return "unknown" if no matching channel was found
//     return "unknown";
// }






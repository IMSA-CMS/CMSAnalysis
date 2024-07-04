#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"
#include "CMSAnalysis/Modules/interface/MultiChannelTreeMakerModule.hh"
#include "TTree.h"

MultiChannelTreeMakerModule::MultiChannelTreeMakerModule(std::shared_ptr<SpecialVariableModule> iSpecialVariable, std::string iName):
TreeMakerModule(iSpecialVariable, iName)
{
    
}

bool MultiChannelTreeMakerModule::process(){
    auto particles = getParticles();
    std::string filter = this->getFilter();
    // std::cout << filter << "\n";

    for (const auto &particle : particles)
    {
        if (trees.find(filter) != trees.end())
        {
            // std::cout<< "Writing";
            this->getSpecialVariable()->calculateVariables(particle);
            trees[filter]->Fill();

        }
    }
    return true;
}

void MultiChannelTreeMakerModule::initialize()
{
    std::vector<std::string> channels = {"eeee_", "eeeu_", "eeuu_", "eueu_", "euuu_", "uuuu_"};
    for(auto channel : channels){
        std::string treeName = channel + getTreeName();
        TTree* channelTree = new TTree(treeName.c_str(), treeName.c_str());//put in name
        getSpecialVariable()->addVariablesToTree(channelTree);
        trees.insert({channel, channelTree});
    }
}
void MultiChannelTreeMakerModule::finalize()
{
    for(auto tree : trees)
    {
       // std::cout<<"Testing " << tree.first;
        tree.second->Write();
    }

}

void MultiChannelTreeMakerModule::fillTree()
{
    for(auto tree : trees){
        tree.second->Fill();
    }
}

std::vector<ParticleCollection<Particle>> MultiChannelTreeMakerModule::getParticles() const
{
    return {getInput()->getParticles(EventInput::RecoLevel::Reco)};
}

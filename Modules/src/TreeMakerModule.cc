#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"
#include "TTree.h"


TreeMakerModule::TreeMakerModule(std::shared_ptr<SpecialVariableModule> iSpecialVariable, std::string iName):
specialVariable(iSpecialVariable),
name(iName)
{
    
}

bool TreeMakerModule::process()
{
    auto leptonJets = getInput()->getSpecial("LeptonJet");
    for (const auto &leptonJet : leptonJets)
    {
        specialVariable->calculateVariables(leptonJet);//enum as parameter?; f
        tree->Fill();
    }
    //specialVariable->calculateVariables();//enum as parameter?; f
    //tree->Fill();
    //auto leptonJets = getInput()->getSpecial("LeptonJet");//do foreach over
    // this like in leptonjetddatastrip, put the prior two lines in at the eend, 
    //MAKE THIS FUNCTION that overrides (new process fucntion in a new class that inherits from TreeMaker module)
    return true;
} 



void TreeMakerModule::initialize()
{
    tree = new TTree(name.c_str(), name.c_str());//put in name
    specialVariable->addVariablesToTree(tree);
} 


void TreeMakerModule::finalize()
{
    tree->Write();
} 


//initialize craetes tree, call fucntion on isepcialtree (addvariables to tree )
    //process calls calcaulte variables to ispeciatree, then can tree->fill from datastripp
    //finalize needs to call tree-> write
//create new class that inherits from specialVariableModule, and pass an object to this thing in the constructor,

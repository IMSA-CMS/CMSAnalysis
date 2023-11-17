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
    specialVariable->calculateVariables();
    tree->Fill();

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

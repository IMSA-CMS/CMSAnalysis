#include "CMSAnalysis/Modules/interface/MLCalculator.hh"
#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"

MLCalculator::MLCalculator(std::shared_ptr<SpecialVariableModule> iSpecialVariable, std::string iFileName, std::string iMethod):
specialVariable(iSpecialVariable),
fileName(Utility::getBasePath() + iFileName),
method(iMethod)
{
    addRequiredModule(specialVariable);
}

void MLCalculator::initialize()
{
    //std::cout << "testing intialize" << "\n\n\n\n\n\n\n\n";
    specialVariable->addVariablesToReader(&reader); //probably something to do with reader parameter? its not initializer in constructor
    reader.BookMVA(method, fileName);
} 

double MLCalculator::getOutput(ParticleCollection<Particle> part) const
{
    specialVariable->calculateVariables(part);
    //std::cout << "MLCalculator temporary float " << *(specialVariable->tempRef1) << '\n';
    //std::cout << "MVA Output\n\n\n" << reader.EvaluateMVA(method);
    return reader.EvaluateMVA(method);
}


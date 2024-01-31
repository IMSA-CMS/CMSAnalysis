#include "CMSAnalysis/Modules/interface/MLCalculator.hh"
#include "CMSAnalysis/Modules/interface/SpecialVariableModule.hh"

MLCalculator::MLCalculator(std::shared_ptr<SpecialVariableModule> iSpecialVariable, std::string iFileName, std::string iMethod):
specialVariable(iSpecialVariable),
fileName(iFileName),
method(iMethod)
{
    
}

void MLCalculator::initialize()
{
    specialVariable->addVariablesToReader(&reader);
    reader.BookMVA(method, fileName);
} 

double MLCalculator::getOutput(Particle part) const
{
    specialVariable->calculateVariables(part);
    return reader.EvaluateMVA(method);
}


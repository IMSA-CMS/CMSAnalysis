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

double MLCalculator::getOutput(ParticleCollection<Particle> part) const
{
    specialVariable->calculateVariables(part);
    //std::cout << "MLCalculator temporary float " << *(specialVariable->tempRef1) << '\n';
    //std::cout << "MVA Output\n\n\n" << reader.EvaluateMVA(method);
    return reader.EvaluateMVA(method);
}


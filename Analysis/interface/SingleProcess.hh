#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include <string>
#include <vector>

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    SingleProcess(std::string iName, double iCrossSection, const Input* iInput) : name(iName), crossSection(iCrossSection), input(iInput) {}
    std::string getName() const {return name;} //Inlining
    double getCrossSection() const {return crossSection;}
    TH1* getHist() const {return input->getInput(name);}

private:
    const std::string name;
    const double crossSection;
    const Input* input;
    Estimator* estimator; 

};




#endif
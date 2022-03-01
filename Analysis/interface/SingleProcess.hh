#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include <string>
#include <vector>
#include "Analysis/interface/Input.hh"

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    std::string getName() const {return name;} //Inlining
    double crossSection() const {return crossSection;}
    TH1* getHist() const {return input->getInput(name);}

private:
    const std::string name;
    const double crossSection;
    const Input* input;
    Estimator* estimator; 

};




#endif
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
    SingleProcess(std::string iName, const Input* iInput) : name(iName), input(iInput) {}
    std::string getName() const {return name;} //Inlining
    std::string getFitName() const {return fitname;} //Inlining
    TH1* getHist() const {return input->getInput(name);}
    TH1* getFitHist() const {return input->getInput(fitname);}



private:
    const std::string name;
    const std::string nickname;
    const std::string fitname;
    const Input* input;
    Estimator* estimator; 

};




#endif
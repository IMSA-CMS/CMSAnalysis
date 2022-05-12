#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include <string>
#include <vector>
#include "CMSAnalysis/Analysis/interface/Input.hh"

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    std::string getName() const {return name;} //Inlining
    std::string getFitName() const {return fitname;} 
    std::string getNickname() const {return nickname;}
    double totalEvents() const {return totalevents;}
    bool getSumSwitch() const {return sumswitch;}
    double getCrossSection() const {return crossSection;}
    TH1* getHist() const {return input->getInput(name);}
    TH1* getFitHist() const {return input->getInput(fitname);}

private:
    const std::string name;
    const std::string nickname;
    const std::string fitname;
    const double totalevents;
    const bool sumswitch;
    const double crossSection;
    const Input* input;
    Estimator* estimator; 

};




#endif
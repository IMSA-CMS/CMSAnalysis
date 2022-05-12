#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include <string>
#include <vector>
<<<<<<< HEAD
#include "CMSAnalysis/Analysis/interface/Input.hh"
=======
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    SingleProcess(std::string iName, double iCrossSection, const Input* iInput) : name(iName), crossSection(iCrossSection), input(iInput) {}
    std::string getName() const {return name;} //Inlining
<<<<<<< HEAD
    std::string getFitName() const {return fitname;} 
    std::string getNickname() const {return nickname;}
    double totalEvents() const {return totalevents;}
    bool getSumSwitch() const {return sumswitch;}
=======
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1
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
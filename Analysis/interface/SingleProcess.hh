#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include "CrossSectionReader.hh"
#include <string>
#include <vector>

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    SingleProcess(std::string iName, const Input* iInput, const CrossSectionReader* iCrossSectionReader) : 
    name(iName), input(iInput), reader(iCrossSectionReader) {}
    std::string getName() const {return name;} //Inlining
    std::string getFitName() const {return fitname;} //Inlining
    TH1* getHist() const {return input->getInput(name);}
    TH1* getFitHist() const {return input->getInput(fitname);}
    int getTotalEvents() const {return input->getTotalEvents();}
    double getCrossSection() const {return reader->getCrossSection(name);}


private:
    const std::string name;
    const std::string nickname;
    const std::string fitname;
    const Input* input;
    const Estimator* estimator; 
    const CrossSectionReader* reader;

};




#endif
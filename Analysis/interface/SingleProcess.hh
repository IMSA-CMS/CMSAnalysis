#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include "CrossSectionReader.hh"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Estimator.hh"
#include "FitEstimator.hh"

class Estimator;

//SingleProcess Class
class SingleProcess
{
public:
    SingleProcess(std::string iCrossSectionName, const std::shared_ptr<Input> iInput, const std::shared_ptr<CrossSectionReader> iCrossSectionReader, const std::shared_ptr<Estimator> iEstimator, const double iLuminosity) : 
    crossSectionName(iCrossSectionName), input(iInput), reader(iCrossSectionReader), estimator(iEstimator), luminosity(iLuminosity) {}
    std::string getName() const {return crossSectionName;} //Inlining
    TH1* getHist(bool scaleToExpected = false) const;
    int getTotalEvents() const;
    double getCrossSection() const {return reader->getCrossSection(crossSectionName);}
    double getExpectedYield() const;
    double getLuminosity() const {return luminosity;}
    std::shared_ptr<Input> getInput() const {return input;}

private:
    const std::string nickname;
    const std::string crossSectionName;
    const std::shared_ptr<Input> input;
    const std::shared_ptr<CrossSectionReader> reader;
    const std::shared_ptr<Estimator> estimator; 
    const double luminosity;
};




#endif
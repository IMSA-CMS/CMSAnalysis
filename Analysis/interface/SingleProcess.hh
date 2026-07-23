#ifndef SINGLEPROCESS_HH
#define SINGLEPROCESS_HH

#include "Input.hh"
#include "CrossSectionReader.hh"
#include "Correction.hh"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Estimator.hh"
#include "FitEstimator.hh"
#include "HistVariable.hh"

class Estimator;

//SingleProcess Class
class SingleProcess
{
    public:
        SingleProcess(std::string iCrossSectionName, const std::shared_ptr<Input> iInput,
            const std::shared_ptr<Estimator> iEstimator) : 
        crossSectionName(iCrossSectionName), input(iInput), estimator(iEstimator) {}
        std::string getName() const {return crossSectionName;}
        TH1* getHist(const HistVariable& histType, bool scaleToExpected = false) const;
        TH1* get2DHist(const HistVariable& histType) const;
        int getTotalEvents() const;
        double getExpectedYield(const HistVariable& dataType) const;
        std::shared_ptr<Input> getInput() const {return input;}

    private:
        const std::string crossSectionName;
        const std::shared_ptr<Input> input;
        const std::shared_ptr<Estimator> estimator; 
};

#endif
